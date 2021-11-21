#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "base45.h"

static const char charset[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', '8',
    '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
    'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
    'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
    ' ', '$', '%', '*', '+', '-', '.', '/', ':'
};

/**
 * @brief Encode a set of raw bytes in Base45
 * 
 * @param dst Destination char array
 * @param src Source char array
 * @param size Bytes to encode
 * @param written Pointer to bytes written to destination
 * @return int 
 */
static int encode(char* dst, char* src, int size, size_t* written)
{
    *written = 0;

    unsigned int sum = 0;
    for (int i = 0; i < size; i++) {
        unsigned char c = src[size - i - 1];
        sum += (unsigned int) (c * pow(256, i));
    }

    int i = 0;
    while (sum > 0) {
        dst[i] = charset[sum % 45];
        sum /= 45;
        (*written)++;
        i++;
    }

    return BASE45_OK;
}

/**
 * @brief Decode set of Base45-encoded bytes into raw bytes
 * 
 * @param dst Destination char array
 * @param src Source char array
 * @param size Bytes to decode
 * @param written Pointer to bytes written to destination
 * @return int 
 */
static int decode(char* dst, char* src, int size, size_t* written)
{
    *written = 0;

    unsigned int sum = 0;
    for (int i = 0; i < size; i++) {
        // check that the input char is a valid Base45 symbol
        if (!strchr(charset, src[i])) {
            return BASE45_INVALID_CHAR;
        }
        char pos = (char) (strchr(charset, src[i]) - charset);
        sum += (unsigned int) (pos * pow(45, i));
    }

    // figure out how many raw bytes are needed to store the Base45 input
    int bytes = (int) ceil(log(sum + 1) / log(256));

    // make sure the max value of the raw bytes does
    // not exceed the max value  of the Base45 input 
    if (pow(256, bytes) > pow(45, size)) {
        return BASE45_INVALID_VALUE;
    }

    for (int i = 0; i < bytes; i++) {
        int power = bytes - i - 1;
        dst[i] = (char) (sum / pow(256, power));
        sum %= (unsigned int) pow(256, power);
        (*written)++;        
    }
    return BASE45_OK;
}

int Base45_encode(char** dest, char* input, size_t length)
{
    char* output = calloc(
        length / 2 * 3 + length % 2 * 2 + 1,
        sizeof(char)
    );

    if (output == NULL) {
        return BASE45_MEMORY_ERROR;
    }
    *dest = output;

    size_t i = 0;
    size_t offset = 0;

    while (i < length) {
        size_t size;

        // consume two raw bytes at a time, except if only one is left
        if (length - i > 1) {
            size = 2;
        } else {
            size = 1;
        }

        size_t written = 0;

        int encode_status = encode(
            *dest + offset,
            input + i,
            size,
            &written
        );

        if (encode_status != 0) {
            free(*dest);
            *dest = NULL;
            return encode_status;
        }

        // add leading zeroes if required
        while (written < size + 1) {
            char* zero = *dest + offset + written;
            *zero = charset[0];
            written++;
        }
        offset += written;

        i += size;
    }

    return BASE45_OK;
}

int Base45_decode(char** dest, char* input, size_t* length)
{
    *length = 0;

    char* output = calloc(
        strlen(input) / 3 * 2 + strlen(input) % 3 / 2 + 1,
        sizeof(char)
    );

    if (output == NULL) {
        return BASE45_MEMORY_ERROR;
    }
    *dest = output;

    size_t i = 0;

    while (i < strlen(input)) {
        int size;

        // consume three encoded bytes at a time, unless only two are left;
        // return error if only one byte is left - this is invalid  Base45
        if (strlen(input) - i > 2) {
            size = 3;
        } else if (strlen(input) - i == 2) {
            size = 2;
        } else {
            free(*dest);
            *dest = NULL;
            *length = 0;
            return BASE45_INVALID_LENGTH;
        }

        size_t written = 0;

        int decode_status = decode(
            *dest + *length,
            input + i,
            size,
            &written
        );

        if (decode_status != 0) {
            free(*dest);
            *dest = NULL;
            *length = 0;
            return decode_status;
        }

        *length += written;
        i += size;
    }

    return BASE45_OK;
}

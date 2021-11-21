#ifndef BASE45_H
#define BASE45_H

enum BASE45_STATUS {
    BASE45_OK,
    BASE45_INVALID_LENGTH,
    BASE45_INVALID_CHAR,
    BASE45_INVALID_VALUE,
    BASE45_MEMORY_ERROR
};

/**
 * @brief Encode data as Base45
 * 
 * @param dest destination for pointer to encoded output
 * @param input pointer to a char array with raw bytes
 * @param length input length
 * @return BASE45_STATUS
 */
int Base45_encode(char** dest, char* input, size_t length);

/**
 * @brief Decode data from Base45
 * 
 * @param dest destination for pointer to decoded output
 * @param input pointer to a char array with Base45-encoded bytes
 * @param length pointer to store number of written bytes
 * @return BASE45_STATUS
 */
int Base45_decode(char** dest, char* input, size_t* length);

#endif

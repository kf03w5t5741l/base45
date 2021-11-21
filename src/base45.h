#ifndef BASE45_H
#define BASE45_H

/**
 * @brief Encode data as Base45
 * 
 * @param dest destination for pointer to encoded output
 * @param input pointer to a char array with raw bytes
 * @param length input length
 * @return 0 if successfull, BASE45_STATUS if unsuccessfull
 */
int Base45_encode(char** dest, char* input, size_t length);

/**
 * @brief Decode data from Base45
 * 
 * @param dest destination for pointer to decoded output
 * @param input pointer to a char array with Base45-encoded bytes
 * @param length pointer to store number of written bytes
 * @return 0 if successfull, BASE45_STATUS if unsuccessfull
 */
int Base45_decode(char** dest, char* input, size_t* length);

#endif

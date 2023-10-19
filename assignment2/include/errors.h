/**
 * @file errors.h
 * @author TODO
 * @author Joshua Lelipaly (s3329941)
 * @author Marc Hoek (s3157520)
 * @brief Token header file
 * @date 2023-09-24
 */
#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>

enum EErrorType {
    UNKNOWN = 1,
    ALLOCATION_ERROR,
    EMPTY_EXPRESSION,
    ILLEGAL_CHARACTER,
    UNPAIRED_LEFT_BRACKET,
    UNPAIRED_RIGHT_BRACKET,
    LAMBDA_MISSING_VAR_AND_EXPRESSION
    //TODO: add more meaningful errors
};

/**
 * @brief prints a descriptive error-message
 *        and exits the program with the matching error-code
 * @param type the type of error that has occured
 */
void throwException(EErrorType type);
#endif //ERRORS_H
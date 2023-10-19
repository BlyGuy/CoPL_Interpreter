/**
 * @file errors.h
 * @author TODO
 * @author Joshua Lelipaly (s3329941)
 * @author Marc Hoek (s3157520)
 * @brief Token header file
 * @date 2023-09-24
 */
#ifndef ERROR_H
#define ERROR_H

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

//TODO: better written messages
static const char* errorMessages[] = 
{
    "An Unknown Error Occured! I am impressed.", //UNKNOWN
    "A memory error occurred. Maybe back down on the parentheses next time ;)", //ALLOCATION_ERROR
    "Empty expressions are not part of the Lambda Calculus language :(", //EMPTY_EXPRESSION
    "(A) Character(s) in the input-string is not allowed to exist :O)", //ILLEGAL_CHARACTER
    "nononono, left bracket(s) are missing a right bracket",      //UNPAIRED_LEFT_BRACKET
    "nononono, u did right bracket before left bracket",      //UNPAIRED_RIGHT_BRACKET
    "A Lambda-character is missing either or both a variable and an expression following the variable" //LAMBDA_MISSING_VAR_AND_EXPRESSION
};

/**
 * @brief prints a descriptive error-message
 *        and exits the program with the matching error-code
 * @param type the type of error that has occured
 */
void throwException(EErrorType type);
#endif //ERROR_H
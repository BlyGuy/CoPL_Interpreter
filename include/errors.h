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
    UNKNOWN,
    ALLOCATION_ERROR
    //TODO: add more meaningful errors
};

static const char* errorMessages[] = 
{
    "An Unknown Error Occured! I am impressed.", //UNKNOWN
    "A memory error occurred. Maybe back down on the parentheses next time ;)" //ALLOCATION_ERROR
};

/**
 * @brief prints a descriptive error-message
 *        and exits the program with the matching error-code
 * @param type the type of error that has occured
 */
void printError(EErrorType type);
#endif //ERROR_H
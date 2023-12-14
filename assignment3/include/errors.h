/**
 * @file errors.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief Header file containing errors
 *        and error-handling functions
 */
#ifndef ERRORS_H
#define ERRORS_H

#include <iostream>

enum EErrorType {
    UNKNOWN,
    ALLOCATION_ERROR,
    ILLEGAL_CHARACTER,
    EMPTY_EXPRESSION,
    LAMBDA_MISSING_VAR_AND_EXPRESSION,
    LAMBDA_MISSING_VAR,
    UNFINISHED_BRACKET_EXPRESSION,
    EMPTY_BRACKET_EXPRESSION,
    UNPAIRED_LEFT_BRACKET,
    UNPAIRED_RIGHT_BRACKET,
    UNFINISHED_PARSING,
    MISSING_COLON,
    ARROW_WITHOUT_HEAD,
    MISSING_ARROW,
    MISSING_SUPERSCRIPT,
    EXPECTED_LVAR,
    EXPECTED_UVAR,
    MISSING_JUDGEMENT_TYPE,
    //TYPE-CHECKING-ERRORS
    UNDECLARED_TYPE,
    APPLICATION_UNRESOLVABLE,
};

/**
 * @brief prints a descriptive error-message
 *        and exits the program with EXIT_FAILURE
 * @param type the type of error that has occured
 */
void throwException(EErrorType type);
#endif //ERRORS_H
/**
 * @file errors.cc
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief A file containing all error-messages and the throwException function
 */
#include "errors.h"

const char* errorMessages[] = 
{
    "Something went wrong during the parsing process. The cause is unknown to the interpreter", //UNKNOWN
    "The program had trouble fetching more memory. This is not your fault", //ALLOCATION_ERROR
    "A Character in the input-string is not allowed to exist", //ILLEGAL_CHARACTER
    "Empty expressions are not part of the lambda calculus language", //EMPTY_EXPRESSION
    "A Lambda-character is missing either or both a variable and an expression following the variable", //LAMBDA_MISSING_VAR_AND_EXPRESSION
    "A left-bracket near the end of the input-string is either missing an expression or following a right bracket", //UNFINISHED_BRACKET_EXPRESSION
    "An expression inside of brackets is not allowed to be empty", //EMPTY_BRACKET_EXPRESSION
    "A left bracket is missing a following right bracket", //UNPAIRED_LEFT_BRACKET
    "A right bracket was entered before a preceding left bracket", //UNPAIRED_RIGHT_BRACKET
    "Parsing ended prematurely. This is caused by a right bracket missing a preceding left bracket" //UNFINISHED_PARSING
};

void throwException(EErrorType type){
    std::cerr << "ERROR: " << errorMessages[type] << " :(" << std::endl;
    exit(EXIT_FAILURE);
} //throwException
#include "errors.h"

//TODO: better written messages
const char* errorMessages[] = 
{
    "An Unknown Error Occured! I am impressed.", //UNKNOWN
    "A memory error occurred. Maybe back down on the parentheses next time ;)", //ALLOCATION_ERROR
    "Empty expressions are not part of the Lambda Calculus language :(", //EMPTY_EXPRESSION
    "(A) Character(s) in the input-string is not allowed to exist :O)", //ILLEGAL_CHARACTER
    "nononono, left bracket(s) are missing a right bracket",      //UNPAIRED_LEFT_BRACKET
    "nononono, u did right bracket before left bracket",      //UNPAIRED_RIGHT_BRACKET
    "A Lambda-character is missing either or both a variable and an expression following the variable" //LAMBDA_MISSING_VAR_AND_EXPRESSION
};

void throwException(EErrorType type){
    std::cerr << "ERROR: " << errorMessages[type-1] << std::endl;
    exit(type);
} //throwException
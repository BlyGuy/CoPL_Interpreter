#include "errors.h"

const char* errorMessages[] = 
{
    "Something went wrong during the parsing process. The cause is unknown to the interpreter", //UNKNOWN
    "The program had trouble fetching more memory. This is not your fault", //ALLOCATION_ERROR
    "A Character in the input-string is not allowed to exist", //ILLEGAL_CHARACTER
    "Empty expressions are not part of the lambda calculus language", //EMPTY_EXPRESSION
    "A Lambda-character is missing either or both a variable and an expression following the variable", //LAMBDA_MISSING_VAR_AND_EXPRESSION
    "A Lambda-character is missing a directly following variable", //LAMBDA_MISSING_VAR
    "A left-bracket near the end of the input-string is either missing an expression or following a right bracket", //UNFINISHED_BRACKET_EXPRESSION
    "An expression inside of brackets is not allowed to be empty", //EMPTY_BRACKET_EXPRESSION
    "A left bracket is missing a following right bracket", //UNPAIRED_LEFT_BRACKET
    "A right bracket was entered before a preceding left bracket", //UNPAIRED_RIGHT_BRACKET
    "Parsing ended prematurely. ADD HERE", //UNFINISHED_PARSING TODO
    "Invalid Judgement, a colon is missing", //MISSING_COLON
    "The base of an arrow '-' is missing a following head '>'",//ARROW_WITHOUT_HEAD
    "Invalid Type, an arrow is missing", //MISSING_ARROW
    "Invalid Abstraction, a superscript is missing", //MISSING_SUPERSCRIPT
    "Variable outside of type-definition must begin with an lowercase letter, uppercase given", //EXPECTED_LVAR
    "Variable in a type-definition must begin with a uppercase letter, lowercase given", //EXPECTED_UVAR
    "A type is missing after the colon ':'", //MISSING_JUDGEMENT_TYPE
    "The type for a variable is unknown, it is most likely not bound to a lambda statement yet", //UNDECLARED_TYPE
    "An application seems to be unresolvable", //APPLICATION_UNRESOLVABLE
};

void throwException(EErrorType type){
    std::cerr << "ERROR: " << errorMessages[type] << " :(" << std::endl;
    exit(EXIT_FAILURE);
} //throwException
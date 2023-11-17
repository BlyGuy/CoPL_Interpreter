/**
 * @file Token.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author N.I. van Laarhoven
 * @brief Token header file
 * @date ????
 */
#ifndef TOKENS_H
#define TOKENS_H

#include <string> 

/**
 * @brief Enum for categorizing tokens
 * @tokens
 *      - single char: LAMBDA '\', LEFT_BRACKET '(' , RIGHT_BRACKET ')'
 *      - multiple char: VAR (regEx = [a-zA-Z][a-zA-Z0-9]*)
 */
enum ETokenType {
   VAR,
   LAMBDA,
   LEFT_BRACKET, RIGHT_BRACKET
};

/**
 * @brief struct representing a token
 */
struct Token {
    ETokenType type;
    std::string varName;
};


#endif //TOKENS_H
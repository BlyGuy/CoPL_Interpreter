/**
 * @file Token.hh
 * @author TODO
 * @author Joshua Lelipaly (s3329941)
 * @author Marc Hoek (s3157520)
 * @brief Token header file
 * @date 2023-09-24
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
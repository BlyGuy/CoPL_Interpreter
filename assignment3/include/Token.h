/**
 * @file Token.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief Token header file
 */
#ifndef TOKENS_H
#define TOKENS_H

#include <string> 

/**
 * @brief Enum for categorizing tokens
 * @tokens
 *      - single char: 
            + LAMBDA '\'
            + LEFT_BRACKET '('
            + RIGHT_BRACKET ')' 
            + COLON ':'
            + SUPERSCRIPT '^'
 *      - multiple char: 
            + LVAR (regEx = [a-z][a-zA-Z0-9]*)
            + UVAR (regEx = [A-Z][a-zA-Z0-9]*)
            + ARROW '->'

 */
enum ETokenType {
   LVAR, UVAR,
   LAMBDA,
   LEFT_BRACKET, RIGHT_BRACKET,
   COLON,
   SUPERSCRIPT,
   ARROW,
};

/**
 * @brief struct representing a token
 */
struct Token {
    ETokenType type;
    std::string varName;
};


#endif //TOKENS_H
/**
 * @file Lexer.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author N.I. van Laarhoven
 * @brief Lexical analyser header file
 * @date ????
 */
#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "Token.h"
#include "errors.h"

class Lexer
{
private:
    /**
    * @brief parses the variable given an input
    * 
    * @param input a string as input
    * @param inputLength the length of the input string
    * @param index the index of the string stream
    * @return the parsed variable as std::string 
    */
    std::string parseVariable(const std::string input, const size_t inputLength, size_t & index);
public:
    Lexer();
    Lexer(std::string input);
    ~Lexer();

    /**
     * @brief converts the input string to tokens
     * 
     * @param input the input string
     */
    void stringToTokens(const std::string input);

    /**
     * @brief prints the token stream
     * 
     */
    void print();

    std::vector<Token> tokens;
};

#endif //LEXER_H
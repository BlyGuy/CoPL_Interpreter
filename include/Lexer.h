#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>
#include "Token.h"
#include "errors.h"

class Lexer
{
private:
    std::string parseVariable(const std::string input, const size_t inputLength, size_t & index);
public:
    Lexer();
    Lexer(std::string input);
    ~Lexer();

    void stringToTokens(const std::string input);
    void print();

    std::vector<Token> tokens;
};

#endif //LEXER_H
/**
 * @file Lexer.cc
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief Implementation of the Lexer class
 */

#include "Lexer.h"

Lexer::Lexer()
{
    
} //Lexer::Lexer

Lexer::Lexer(std::string input)
{
    stringToTokens(input);
} //Lexer::Lexer(std::string)

Lexer::~Lexer()
{
    tokens.clear();
} //Lexer::~Lexer

std::string Lexer::parseVariable(const std::string input, const size_t inputLength, size_t & index)
{
    size_t oldIndex = index;
    char inputChar;
    for (; index < inputLength; index++)
    {
        inputChar = input[index];
        if(!((inputChar >= 'a' && inputChar <= 'z') 
        || (inputChar >= 'A' && inputChar <= 'Z') 
        || (inputChar >= '0' && inputChar <= '9'))) {
            break; //break if not alphanumberic
        }
    }
    return input.substr(oldIndex, index - oldIndex);
} //Lexer::parseVariable

void Lexer::stringToTokens(const std::string input)
{
    const size_t inputLength = input.length();
    char inputChar;
    //TODO: Implement dot-tokens
    //size_t dotBrackets = 0;
    for (size_t i = 0; i < inputLength; i++)
    {
        inputChar = input[i];
        switch (inputChar)
        {
        case '\\':
            tokens.push_back({LAMBDA, ""});
            break;
        case '(':
            tokens.push_back({LEFT_BRACKET, ""});
            break;
        case ')':
            tokens.push_back({RIGHT_BRACKET, ""});
            break;
        case ':':
            tokens.push_back({COLON, ""});
            break;
        case '^':
            tokens.push_back({SUPERSCRIPT, ""});
            break;
        case '-':
            if (i + 1 < inputLength && input[i+1] == '>')
            {
                tokens.push_back({ARROW, ""});
                i++;
            } else {
                throwException(ARROW_WITHOUT_HEAD);
            }
            break;
        //Ignore Whitespace
        case '\n':
        case '\r':
        case '\t':
        case ' ':
            break;    
        default:
            //If the character is alphabetical
            if (inputChar >= 'a' && inputChar <= 'z') { //lower-case
                tokens.push_back({LVAR, parseVariable(input, inputLength, i)});
                i--; //to prevent the lexer from skipping a character
            }
            else if (inputChar >= 'A' && inputChar <= 'Z') { //upper-case
                tokens.push_back({UVAR, parseVariable(input, inputLength, i)});
                i--; //to prevent the lexer from skipping a character
            } else {
                std::cerr << "The character in question: " << input[i] 
                << ", character-index in string: " << i << std::endl;
                throwException(ILLEGAL_CHARACTER);
            }
        }
    }
} //Lexer::stringToTokens

void Lexer::print()
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        switch (tokens[i].type)
        {
        case LVAR: case UVAR:
            std::cout << tokens[i].varName << ' ';
            break;
        case LAMBDA:
            std::cout << "\\";
            break;
        case LEFT_BRACKET:
            std::cout << "( ";
            break;
        case RIGHT_BRACKET:
            std::cout << ") ";
            break;
        case COLON:
            std::cout << ": ";
            break;
        case SUPERSCRIPT:
            std::cout << "^ ";
            break;
        case ARROW:
            std::cout << "-> ";
            break;
        default:
            break;
        }
    }
    std::cout << std::endl;
}
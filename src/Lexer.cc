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
    //long long openBrackets = 0;
    //TODO: Optional feature for more points
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
            //openBrackets++;
            tokens.push_back({LEFT_BRACKET, ""});
            break;
        case ')':
            tokens.push_back({RIGHT_BRACKET, ""});
            //openBrackets--;
            // if (openBrackets < 0) {
            //     std::cout << "character-index in string: " << i << std::endl;
            //     printError(UNPAIRED_RIGHT_BRACKET);
            // }
            break;
        //Ignore Whitespace
        case '\n':
        case '\r':
        case '\t':
        case ' ':
            break;    
        default:
            //If the character is alphabetical
            if ((inputChar >= 'a' && inputChar <= 'z') || (inputChar >= 'A' && inputChar <= 'Z')) {
                tokens.push_back({VAR, parseVariable(input, inputLength, i)});
                i--;//to prevent the lexer from skipping a character
            } else {
                std::cout << "The character in question: " << input[i] 
                << ", character-index in string: " << i << std::endl;
                printError(ILLEGAL_CHARACTER);
            }
        }
    }
    
    // if (openBrackets > 0) {
    //     std::cout << "Unpaired Left Brackets: " << openBrackets << std::endl;
    //     printError(UNPAIRED_LEFT_BRACKET);
    // }
} //Lexer::stringToTokens

void Lexer::print()
{
    for (size_t i = 0; i < tokens.size(); i++)
    {
        switch (tokens[i].type)
        {
        case VAR:
            std::cout << tokens[i].varName << ' ';
            break;
        case LAMBDA:
            std::cout << "\\";
            break;
        case LEFT_BRACKET:
            std::cout << "( ";
            break;
        case RIGHT_BRACKET :
            std::cout << ") ";
            break;
        default:
            break;
        }
    }
    std::cout << std::endl;
}
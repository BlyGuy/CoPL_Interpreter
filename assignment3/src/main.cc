#include <iostream>
#include <string>
#include <vector>
#include "errors.h"
#include "Token.h"
#include "Lexer.h"
#include "SyntaxTree.h"

int main() {
    std::string input;
    while (std::getline(std::cin, input) && input != "exit")
    {
        //Lexical analysis
        Lexer lex(input); //convert input text into tokenStream;
        std::cout << "Lexer: ";
        lex.print();

        //Syntactical analysis
        SyntaxTree tree;

        //convert tokenStream to SyntaxTree
        if (!tree.constructParseTree(lex.tokens)) {
            //Invalid expression: print error and exit
            throwException(UNKNOWN);
        }
        //The expression is grammatically valid

        //TODO: IMPLEMENT
        // if (!tree.typeValidation()) {
        //     throwException(UNKNOWN); //TODO: Unresolvable expression error
        // }
        
        std::cout << "tree:  ";
        tree.print();
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}
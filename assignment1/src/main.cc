/**
 * @file main.cc
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief Main file for testing the syntactical analyser
 */
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
        //lex.print();

        //Syntactical analysis
        SyntaxTree tree;

        //convert tokenStream to SyntaxTree
        if (!tree.constructParseTree(lex.tokens)) {
            //Invalid expression: print error and exit
            throwException(UNKNOWN);
        } else {
            //Your expression is valid :)
            tree.print();
        }
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}
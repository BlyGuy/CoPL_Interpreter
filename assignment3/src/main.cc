/**
 * @file main.cc
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief main function that checks the derivability of judgements in a file
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "errors.h"
#include "Token.h"
#include "Lexer.h"
#include "SyntaxTree.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Incorrect parameters! :(\n" <<
        "Usage: ./type-checker <file containing judgement(s)>\n"
        "A file must contain one complete judgement per line!" << std::endl;
        return EXIT_FAILURE;
    }
    
    std::ifstream inputFile(argv[1]);
    if(!inputFile){
        std::cerr << "Inputfile could not be opened :("  << std::endl;
        return EXIT_FAILURE;
    }
    
    std::string input;
    while (std::getline(inputFile, input) && input != "exit")
    {
        //Perform lexical analysis
        //convert input text into tokenStream
        Lexer lex(input);

        //The expression is lexically valid
        /* std::cout << "Lexer: ";
        lex.print(); */

        //Perform syntactical analysis
        //convert tokenStream to SyntaxTree
        SyntaxTree tree;
        if (!tree.constructParseTree(lex.tokens)) {
            //Invalid expression: print error and exit
            throwException(UNKNOWN);
        }

        //The expression is syntactically valid
        /* std::cout << "tree:  ";
        tree.print();
        std::cout << std::endl; */

        //Perform type checking
        if (!tree.typeCheck()) {
            throwException(TYPE_CHECK_FAILED);
        }
        //The judgement is derivable
        tree.print();
        std::cout << std::endl;
    }
    return EXIT_SUCCESS;
}
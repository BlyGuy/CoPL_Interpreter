/**
 * @file main.cc
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief main file for testing the interpreter
 * @date ????
 */
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "errors.h"
#include "Token.h"
#include "Lexer.h"
#include "SyntaxTree.h"

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Incorrect parameters :( !" <<
        " Usage: ./interpreter <file containing expression>" << std::endl;
        return EXIT_FAILURE;
    }


    std::string input;

    //reads input file
    std::ifstream inputfile(argv[1], std::ios::binary);
	if (inputfile) {
		inputfile.seekg(0, std::ios::end);
		input.resize(inputfile.tellg());
		inputfile.seekg(0, std::ios::beg);
		inputfile.read(&input[0], input.size());
		inputfile.close();
	} else {
        std::cerr << "Inputfile cannot be opened :("  << std::endl;
        return EXIT_FAILURE;
    }

    //Lexical analysis
    Lexer lex(input); //convert input text into tokenStream;
    //lex.print();

    //Syntactical analysis
    SyntaxTree tree;

    //convert tokenStream to SyntaxTree
    if (!tree.constructParseTree(lex.tokens)) {
        //Invalid expression: print error and exit
        throwException(UNKNOWN);
    }

    //The expression is valid -> perform reductions
    bool noLimitReached = tree.reduce();
    //Print the final reduced expression
    tree.print();
    std::cout << std::endl;
    //Exit with the right exit-code
    if (noLimitReached)
        return EXIT_SUCCESS;
    else
        return EXIT_LIMIT_REACHED;
}
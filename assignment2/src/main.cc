#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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
    } else {
        //Your expression is valid :)
        tree.print();
    }
    std::cout << std::endl;

    //TODO: reduce parse tree
    //tree.reduce();
    //tree.print();
    //std::cout << std::endl;
    return EXIT_SUCCESS;
}
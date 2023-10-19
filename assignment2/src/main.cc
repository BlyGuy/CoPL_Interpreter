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
        std::cout << "Tokenstring: ";
        Lexer lex(input); //convert input text into tokenStream;
        lex.print();

        //Syntactical analysis
        std::cout << "SyntaxTree: ";
        SyntaxTree tree; //convert tokenStream to SyntaxTree;
        if (!tree.constructParseTree(lex.tokens)) {
            std::cout << "Er ging iets mis tijdens het parsen :(" << std::endl;
        } else {
            tree.print();
        }
        std::cout << std::endl;

        //TODO: implent this
        //Semantical analysis
        // tree.semanticalAnalysis(); //decorate the syntaxTree
        // tree.optimize();
        // std::cout << "Optimized tree: ";
        // tree.print();

    }
    return 0;
}
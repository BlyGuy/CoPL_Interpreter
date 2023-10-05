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
        SyntaxTree tree(lex); //convert tokenStream to SyntaxTree;
        tree.print();

        //Semantical analysis
        // tree.semanticalAnalysis(); //decorate the syntaxTree
        // tree.optimize();
        // std::cout << "Optimized tree: ";
        // tree.print();

        //Interpretation step
        //blab.ExecuteParseTree();
    }
    return 0;
}
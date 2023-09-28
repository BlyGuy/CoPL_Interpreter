#include "SyntaxTree.h"

SyntaxTree::SyntaxTree()
{
    root = nullptr;
}

SyntaxTree::~SyntaxTree()
{
    //TODO!!!
}

SyntaxTree::SyntaxTree(Lexer & lex)
{
    ConstructParseTree(lex);
}

bool SyntaxTree::ConstructParseTree(Lexer & lex)
{
    if (lex.tokens.empty()) {
        printError(EMPTY_EXPRESSION);
    }
} //Syntaxtree::ConstructParseTree

bool SyntaxTree::ConstructParseTreeApp(Lexer & lex)
{
    
} //SyntaxTree::ConstructParseTreeSub

bool SyntaxTree::ConstructParseTreeAbstr(Lexer & lex)
{
    
} //SyntaxTree::ConstructParseTreeSub

bool SyntaxTree::ConstructParseTreeFactor(Lexer & lex)
{
    
} //SyntaxTree::ConstructParseTreeSub

void SyntaxTree::print(){
    std::cout << "Print-functie nog niet geïmplementeerd :/" << std::endl;
} //SyntaxTree::print

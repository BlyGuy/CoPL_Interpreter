#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <iostream>
#include "Lexer.h"
#include "errors.h"

enum ENodeType {
    APPLICATION,
    ABSTRACTION,
    FACTOR
};

struct Node {
    ENodeType type;
    std::string varName = ""; //in case of leaf-FACTOR-node
    Node* left = nullptr;
    Node* right = nullptr;
};

class SyntaxTree
{
private:
    Token* root;

    bool ConstructParseTree(Lexer & lex);
    bool ConstructParseTreeApp(Lexer & lex);
    bool ConstructParseTreeAbstr(Lexer & lex);
    bool ConstructParseTreeFactor(Lexer & lex);
public:
    SyntaxTree();
    SyntaxTree(Lexer & lex);
    ~SyntaxTree();

    void print();
};


#endif //SYNTAXTREE_H
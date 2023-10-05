#ifndef SYNTAXTREE_H
#define SYNTAXTREE_H

#include <iostream>
#include "Lexer.h"
#include "errors.h"

/**
 * Unambiguous grammar definition:
 * 
 * <Application>    ::= <Application> <Abstraction> 
 *                    | <Abstraction>
 * <Abstraction>    ::= LAMBDA VAR <Atomic>
 *                    | <Atomic>
 * <Atomic>         ::= LEFT_BRACKET <Application> RIGHT_BRACKET
 *                    | VAR
 */

enum ENodeType {
    APPLICATION,
    ABSTRACTION,
    ATOMIC
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
    Node* root;

    bool ConstructParseTree(Lexer & lex);
    bool ConstructParseTreeApp(Lexer & lex, size_t & index, Node* subTree);
    bool ConstructParseTreeAbstr(Lexer & lex, size_t & index, Node* subTree);
    bool ConstructParseTreeFactor(Lexer & lex, size_t & index, Node* subTree);

    void print(Node* node);
public:
    SyntaxTree();
    SyntaxTree(Lexer & lex);
    ~SyntaxTree();

    void print();
};


#endif //SYNTAXTREE_H
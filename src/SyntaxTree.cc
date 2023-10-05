#include "SyntaxTree.h"

SyntaxTree::SyntaxTree()
{
    root = nullptr;
} //SyntaxTree::SyntaxTree

SyntaxTree::~SyntaxTree()
{
    //TODO!!!
} //SyntaxTree::~SyntaxTree

SyntaxTree::SyntaxTree(Lexer & lex)
{
    ConstructParseTree(lex); //Creëert Segmentation fault! :(
} //SyntaxTree::SyntaxTree(Lexer)

bool SyntaxTree::ConstructParseTree(Lexer & lex)
{
    if (lex.tokens.empty()) {
        printError(EMPTY_EXPRESSION);
        return false;
    }

    size_t index = lex.tokens.size();
    if (ConstructParseTreeApp(lex, index, root)){
        return true;
    }
    return false;
} //Syntaxtree::ConstructParseTree

bool SyntaxTree::ConstructParseTreeApp(Lexer & lex, size_t & index, Node* subTree)
{
    if (index < 0) {
        return false;
    }
    subTree->right = new Node;
    if (subTree->left == nullptr) {
        printError(ALLOCATION_ERROR);
        return false;
    }
    subTree->right->type = ABSTRACTION;
    if (!ConstructParseTreeAbstr(lex, index, subTree)) {
        return false;
    } else if (index < 0) {
        return true;
    }
    subTree->left = new Node;
    if (subTree->left == nullptr) {
        printError(ALLOCATION_ERROR);
        return false;
    }
    subTree->left->type = APPLICATION;
    index--;
    return ConstructParseTreeApp(lex, index, subTree->right);
} //SyntaxTree::ConstructParseTreeSub

bool SyntaxTree::ConstructParseTreeAbstr(Lexer & lex, size_t & index, Node* subTree)
{
    subTree->left = new Node;
    if (subTree->left == nullptr) {
        printError(ALLOCATION_ERROR);
        return false;
    }
    subTree->left->type = ATOMIC;
    return true;
} //SyntaxTree::ConstructParseTreeSub

bool SyntaxTree::ConstructParseTreeFactor(Lexer & lex, size_t & index, Node* subTree)
{
    return true;
} //SyntaxTree::ConstructParseTreeSub

void SyntaxTree::print() {
    print(root);
}

void SyntaxTree::print(Node* node)
{
    if (node == nullptr) {
        return;
    }
    switch (node->type)
    {
    case APPLICATION:
        print(node->left);
        print(node->right);
        break;
    case ABSTRACTION:
        if (!node->varName.empty()) {
            std::cout << node->varName << ' ';
        } else {
            print(node->left);
        }
        break;
    case ATOMIC:
        if (!node->varName.empty()) {
            std::cout << node->varName << ' ';
        } else {
            std::cout << "( ";
            print(node->left);
            std::cout << ") ";
        }
        break;
    }
} //SyntaxTree::print

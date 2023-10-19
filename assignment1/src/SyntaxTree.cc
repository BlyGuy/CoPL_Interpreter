#include "SyntaxTree.h"

// NODE-CLASS FUNCTIONS //

Node::~Node() {
    if (left != nullptr) {
        delete left;
    }
    if (right != nullptr) {
        delete right;
    }
}


// SYNTAXTREE-CLASS FUNCTIONS //

SyntaxTree::SyntaxTree()
{
    root = nullptr;
} //SyntaxTree::SyntaxTree

SyntaxTree::~SyntaxTree()
{
    clear();
} //SyntaxTree::~SyntaxTree

SyntaxTree::SyntaxTree(Lexer & lex)
{
    ConstructParseTree(lex.tokens); //Creëert Segmentation fault! :(
} //SyntaxTree::SyntaxTree(Lexer)


bool SyntaxTree::ConstructParseTree(std::vector<Token> & tokens)
{
    if (tokens.empty()) {
        throwException(EMPTY_EXPRESSION);
    }
    //Allocate the root-node
    root = new Node;
    if (root == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    //Parse the first application
    root->type = APPLICATION;
    size_t index = 0;
    bool result = ConstructParseTreeApp(tokens, index, root);

    //Checking if parsing finished prematurely
    if (index < tokens.size()) {
        //this happens due to a unpaired right-bracket in the top layer
        throwException(UNPAIRED_RIGHT_BRACKET);
    }
    return result;
} //Syntaxtree::ConstructParseTree


bool SyntaxTree::ConstructParseTreeApp(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    //Parse the Abstraction
    subTree->left = new Node;
    if (subTree->left == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    subTree->left->type = ABSTRACTION;
    if (!ConstructParseTreeAbstr(tokens, index, subTree->left)) {
        return false;
    } 

    //Check the next look-ahead token if we're only parsing an abstraction
    index++;
    if (index >= tokens.size() || tokens[index].type == RIGHT_BRACKET) {
        return true;
    }
    //Parse the next Application
    subTree->right = new Node;
    if (subTree->right == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    subTree->right->type = APPLICATION;
    return ConstructParseTreeApp(tokens, index, subTree->right);
} //SyntaxTree::ConstructParseTreeSub

bool SyntaxTree::ConstructParseTreeAbstr(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    //Check for a lambda-expression and parse one if needed
    if (tokens[index].type == LAMBDA) {
        if (!(index + 2 < tokens.size() && tokens[index + 1].type == VAR)) {
            throwException(LAMBDA_MISSING_VAR_AND_EXPRESSION);
        }
        //Store the Lambda-variable directly in the node
        subTree->varName = tokens[index + 1].varName;
        //Parse the following Abstraction
        index += 2;
        subTree->left = new Node;
        if (subTree->left == nullptr) {
            throwException(ALLOCATION_ERROR);
        }
        subTree->left->type = ABSTRACTION;
        return ConstructParseTreeAbstr(tokens, index, subTree->left);
    }

    //Parse the Atomic
    subTree->left = new Node;
    if (subTree->left == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    subTree->left->type = ATOMIC;
    return ConstructParseTreeAtom(tokens, index, subTree->left);
} //SyntaxTree::ConstructParseTreeSub


bool SyntaxTree::ConstructParseTreeAtom(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    if (tokens[index].type == VAR) {
        subTree->varName = tokens[index].varName;
        return true;
    }

    if (tokens[index].type == LEFT_BRACKET) {
        //Check for empty expression
        if (index + 1 < tokens.size() && tokens[index + 1].type == RIGHT_BRACKET) {
            throwException(EMPTY_BRACKET_EXPRESSION);
        }
        //Parse the sub-expression
        index++;
        subTree->left = new Node;
        if (subTree->left == nullptr) {
            throwException(ALLOCATION_ERROR);
        }
        subTree->left->type = APPLICATION;
        if (!ConstructParseTreeApp(tokens, index, subTree->left)){
            return false;
        }
        // Check for a matching right_bracket
        if (tokens[index].type != RIGHT_BRACKET) {
            throwException(UNPAIRED_LEFT_BRACKET);
        }
        return true;
    }

    if (tokens[index].type == RIGHT_BRACKET) {
        throwException(UNPAIRED_RIGHT_BRACKET);
    }

    return false;
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
        if (!(node->varName.empty())) {
            std::cout << '\\' << node->varName << ' ';
        }
        print(node->left);
        break;
    case ATOMIC:
        if (!(node->varName.empty())) {
            std::cout << node->varName << ' ';
        } else {
            std::cout << "( ";
            print(node->left);
            std::cout << ") ";
        }
        break;
    }
} //SyntaxTree::print


void SyntaxTree::clear()
{
    if (root == nullptr) {
        return;
    }
    delete root;
    root = nullptr;
} //SyntaxTree::clear
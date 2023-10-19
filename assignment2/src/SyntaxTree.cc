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
    constructParseTree(lex.tokens); //Creëert Segmentation fault! :(
} //SyntaxTree::SyntaxTree(Lexer)


bool SyntaxTree::constructParseTree(std::vector<Token> & tokens)
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
    size_t symTableIndex = 0;
    bool result = constructParseTreeApp(tokens, index, symTableIndex, root);

    //The first application is not allowed to end with a right_bracket
    if (tokens[index].type == RIGHT_BRACKET) {
        throwException(UNPAIRED_RIGHT_BRACKET);
    }
    return result;
} //Syntaxtree::ConstructParseTree


bool SyntaxTree::constructParseTreeApp(std::vector<Token> & tokens, size_t & index, size_t & symTableIndex, Node* subTree)
{
    //Parse the Abstraction
    subTree->left = new Node;
    if (subTree->left == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    subTree->left->type = ABSTRACTION;
    if (!constructParseTreeAbstr(tokens, index, symTableIndex, subTree->left)) {
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
    return constructParseTreeApp(tokens, index, symTableIndex, subTree->right);
} //SyntaxTree::ConstructParseTreeSub

bool SyntaxTree::constructParseTreeAbstr(std::vector<Token> & tokens, size_t & index, size_t & symTableIndex, Node* subTree)
{
    //Check for a lambda-expression and parse one if needed
    if (tokens[index].type == LAMBDA) {
        if (!(index + 2 < tokens.size() && tokens[index + 1].type == VAR)) {
            throwException(LAMBDA_MISSING_VAR_AND_EXPRESSION);
        }
        std::cout << "hallo" << std::endl;
        //Connect the Lambda-variable directly to the node
        //And create a new SymTableEntry for this variable
        symTable.push_back({tokens[index + 1].varName, true, symTableIndex});
        subTree->symTableID = symTableIndex;
        symTableIndex++;
        //Parse the following Abstraction
        index += 2;
        subTree->left = new Node;
        if (subTree->left == nullptr) {
            throwException(ALLOCATION_ERROR);
        }
        subTree->left->type = ABSTRACTION;
        return constructParseTreeAbstr(tokens, index, symTableIndex, subTree->left);
    }

    //Parse the Atomic
    subTree->left = new Node;
    if (subTree->left == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    subTree->left->type = ATOMIC;
    return constructParseTreeAtom(tokens, index, symTableIndex, subTree->left);
} //SyntaxTree::ConstructParseTreeSub


bool SyntaxTree::constructParseTreeAtom(std::vector<Token> & tokens, size_t & index, size_t & symTableIndex, Node* subTree)
{
    if (tokens[index].type == VAR) {
        //Create a new SymTableEntry for the variable
        symTable.push_back({tokens[index].varName, false, symTableIndex});
        subTree->symTableID = symTableIndex;
        symTableIndex++;
        return true;
    }

    if (tokens[index].type == LEFT_BRACKET) {
        // if (index < tokens.size && tokens[index + 1] == RIGHT_BRACKET) {
        //     throwException(EMPTY_BRACKET_EXPRESSION);
        // }
        //Parse the sub-expression
        index++;
        subTree->left = new Node;
        if (subTree->left == nullptr) {
            throwException(ALLOCATION_ERROR);
        }
        subTree->left->type = APPLICATION;
        if (!constructParseTreeApp(tokens, index, symTableIndex, subTree->left)){
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

bool SyntaxTree::converse(Node* subTree, Token & var, std::string subs) 
{
    //var.varName = 
} //SyntaxTree::converse

bool SyntaxTree::reduce(Node* subTreeSubst, Node* subTreeExpr, Node* subTreeVar, Node* subTreeRoot)
{
    //TODO!!!
    //Loop variables af in M
    //  - Hoe? recursief lol
    //Bij variable -> perform substitution 
    //      if bound == true && lambdaVar = var in M 
    //      (wss niet boundID = het ID van M )
    // 
    //  - Hoe? Wishful thinking of niet
    //      - WT: Laat de parent direct naar N wijzen
    //      - Niet: Laat de parent naar een kopie van N wijzen
    
    subTreeVar = subTreeSubst;               //x:=N
    subTreeRoot->left->left = subTreeRoot->left; //
    subTreeRoot->right = nullptr;
} //SyntaxTree::reduce

void SyntaxTree::print() {
    print(root);
} //SyntaxTree::print


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
        if (!(symTable[node->symTableID].varName.empty())) {
            // std::cout << node->symTableID;
            std::cout << '\\' << symTable[node->symTableID].varName << ' ';
        }
        print(node->left);
        break;
    case ATOMIC:
        if (!(symTable[node->symTableID].varName.empty())) {
            // std::cout << node->symTableID;
            std::cout << symTable[node->symTableID].varName << ' ';
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
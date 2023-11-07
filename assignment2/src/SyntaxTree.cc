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
    constructParseTree(lex.tokens);
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
    bool result = constructParseTreeApp(tokens, index, root);

    //Checking if parsing finished prematurely
    if (index < tokens.size()) {
        //this happens due to a unpaired right-bracket in the top layer
        throwException(UNPAIRED_RIGHT_BRACKET);
    }
    return result;
} //Syntaxtree::constructParseTree


bool SyntaxTree::constructParseTreeApp(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    //Parse the Abstraction
    subTree->left = new Node;
    if (subTree->left == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    subTree->left->type = ABSTRACTION;
    if (!constructParseTreeAbstr(tokens, index, subTree->left)) {
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
    return constructParseTreeApp(tokens, index, subTree->right);
} //SyntaxTree::constructParseTreeApp

bool SyntaxTree::constructParseTreeAbstr(std::vector<Token> & tokens, size_t & index, Node* subTree)
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
        return constructParseTreeAbstr(tokens, index, subTree->left);
    }

    //Parse the Atomic
    // subTree->left = new Node;
    // if (subTree->left == nullptr) {
    //     throwException(ALLOCATION_ERROR);
    // }
    // subTree->left->type = ATOMIC;
    subTree->type = ATOMIC;
    //return constructParseTreeAtom(tokens, index, subTree->left);
    return constructParseTreeAtom(tokens, index, subTree);
} //SyntaxTree::constructParseTreeAbstr


bool SyntaxTree::constructParseTreeAtom(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    if (tokens[index].type == VAR) {
        subTree->varName = tokens[index].varName;
        return true;
    }

    if (tokens[index].type == LEFT_BRACKET) {
        if (index + 1 < tokens.size() && tokens[index + 1].type == RIGHT_BRACKET) {
            throwException(EMPTY_BRACKET_EXPRESSION);
        }
        if (index + 1 >= tokens.size()) {
            throwException(UNPAIRED_LEFT_BRACKET);
        }
        //Parse the sub-expression
        index++;
        subTree->left = new Node;
        if (subTree->left == nullptr) {
            throwException(ALLOCATION_ERROR);
        }
        subTree->left->type = APPLICATION;
        if (!constructParseTreeApp(tokens, index, subTree->left)){
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
} //SyntaxTree::constructParseTreeAtom

bool SyntaxTree::converse(Node* subTree, Token & var, std::string subs) 
{
    //var.varName = 
    return false;
} //SyntaxTree::converse

void SyntaxTree::reduce()
{
    
    /**
     * @brief free parking
     *  | 🚘 | 🚘 | 🚘 | 🚚 | 🗿
     * 
     *  | 🚘 |    |    |    |    | 🚘 | 
     * 
     *  | 🚘 | 🚘 | 🚘 | 🚘 | 🚚🚚🚚|
     * 
     *  | 🚘 | 🐟 | 🚜 | 🦼 | 🚛 | i |
     * 
     *  | 🚘 | 🚎 | 🚒 | 🚁 | 🚀 | 🛸 | 
     * 
     *  |🚲🚲🚲🚲|   | 🚢 | 🚘 | ⚽🤶
     * 
     * 🚏 🕺 🚎
     */

     
    //TODO!!!

    int count = 0;
    int index = 0;
    bool reducePossible = true;
    while (reducePossible == true && count < MAX_REDUCE )
    {
        /**
         * @brief M en N
         * M is een lambda abstraction
         * N is een application op M (indirect of direct maar de eerste)
         */
        
        Node* M, N;
        /**
         * @brief vind M
         * bij app: eerst links kijken dan rechts
         * bij lambda: is het daadwerkelijk een lambda zo ja bingo
         * anders kijk je naar atomic daaronder
         * bij atom: is het een var, jammer
         *  zo nee, check bracket expr
         * M is een lambda abstraction
         * N is een application op M (indirect of direct maar de eerste)
         */
        //
        //vind M en N
        //result = findMN(root);
    }
    
    //Loop variables af in M
    //  - Hoe? recursief lol
    //Bij variable -> perform substitution 
    //      if bound == true && lambdaVar = var in M 
    //      (wss niet boundID = het ID van M )
    // 
    //  - Hoe? Wishful thinking of niet
    //      - WT: Laat de parent direct naar N wijzen
    //      - Niet: Laat de parent naar een kopie van N wijzen
    
    //reduceSub();
}

Node* SyntaxTree::findLambda(Node* subtree)
{
    if (subtree == nullptr) {
        return nullptr;
    }

    Node* lambda = nullptr;
    
    switch (subtree->type)
    {
    case APPLICATION:
        lambda = findLambda(subtree->left);
        if (lambda == nullptr)
            return findLambda(subtree->right);
        else 
            return lambda;
    case ABSTRACTION:
        if (!(subtree->varName.empty())) {
            return subtree;
        }
        break;
        // return true;
    case ATOMIC:
        return findLambda(subtree->left);
    default:
        return nullptr;
    }
} //SyntaxTree::findLambda


bool SyntaxTree::reduceSub(Node* subTreeSubst, Node* subTreeExpr, Node* subTreeVar, Node* subTreeRoot)
{
    switch (subTreeRoot->type)
    {
    case APPLICATION:
        // reduceSub(subTreeRoot->left);
        // reduceSub(subTreeRoot->right);
        break;
    case LAMBDA:
    
    default:
        break;
    }


    // subTreeVar = subTreeSubst;               //x:=N
    // subTreeRoot->left->left = subTreeRoot->left; //
    // subTreeRoot->right = nullptr;
    return false;
} //SyntaxTree::reduceSub

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
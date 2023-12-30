/**
 * @file SyntaxTree.cc
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief Implementation of the Node and SyntaxTree class
 */

#include "SyntaxTree.h"

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
        throwException(UNFINISHED_PARSING);
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

// ALPHA-CONVERSION FUNCTIONS //

void SyntaxTree::alphaConverse(Node* M, const Node* N) 
{
    std::vector<std::string> Mbound, Nbound, conflicts;
    getBoundVars(Mbound, M);
    getBoundVars(Nbound, N);
    getConflictVars(conflicts, Mbound, Nbound, N);

    if (!conflicts.empty()) {
        resolveConflicts(conflicts, Mbound, M);
    }
} //SyntaxTree::alphaConverse

void SyntaxTree::getBoundVars(std::vector<std::string> & result, const Node* subTree)
{
    if (subTree == nullptr) return;

    if (subTree->type == ABSTRACTION)
        result.push_back(subTree->varName); //bound
    
    getBoundVars(result, subTree->left);
    getBoundVars(result, subTree->right);
} //SyntaxTree::getBoundVars

void SyntaxTree::getConflictVars(std::vector<std::string> & result, const std::vector<std::string> Mbound, const std::vector<std::string> Nbound, const Node* subTree)
{
    if (subTree == nullptr) return;

    if (subTree->type == ATOMIC && !subTree->varName.empty() &&
        containsString(Mbound, subTree->varName) &&
        !containsString(Nbound, subTree->varName))
        result.push_back(subTree->varName); //conflict
    
    getConflictVars(result, Mbound, Nbound, subTree->left);
    getConflictVars(result, Mbound, Nbound, subTree->right);
} //SyntaxTree::getConflictVars

void SyntaxTree::resolveConflicts(const std::vector<std::string> & conflicts, const std::vector<std::string> Mbound, Node* subTree)
{
    if (subTree == nullptr) return;

    if (!(subTree->varName.empty())) {
        for (size_t i = 0; i < conflicts.size(); i++) {
            if (subTree->varName == conflicts[i]) {
                size_t index = subTree->varName.size();
                subTree->varName += 'A';
                //Append a substring so the resulting varName is not present in Mbound
                while(containsString(Mbound, subTree->varName)) {
                    if (subTree->varName[index] >= 'Z')
                        subTree->varName[index] = 'a';
                    else if (subTree->varName[index] >= 'z') {
                        subTree->varName += 'A';
                        index++;
                    }
                    subTree->varName[index] += 1; //go to next letter
                }
                break;
            }
        }
    }
    resolveConflicts(conflicts, Mbound, subTree->left);
    resolveConflicts(conflicts, Mbound, subTree->right);
} //SyntaxTree::resolveConflicts

// BETA-REDUCTION FUNCTIONS //

bool SyntaxTree::reduce()
{   
    int i = 0; // Reduction counter
    bool reducePossible = true;
    
    for (i = 0; reducePossible && i < MAX_REDUCTIONS; i++)
    {
        reducePossible = reduceSub(root);
        clean();
    }
    return !reducePossible; //Indicates if the limit was reached
} //SyntaxTree::reduceTree

bool SyntaxTree::reduceSub(Node* subTree)
{
    if (subTree == nullptr)
        return false; // No reduction possible
    
    
    if (subTree->type == APPLICATION)
    {
        bool reductionHappened = false;
        // Recursively reduce its children
        reductionHappened = reduceSub(subTree->left);
        reductionHappened |= reduceSub(subTree->right);
        // Reduce the parent
        if (subTree->right != nullptr) {
            Node* parent = findLambdaParent(subTree);
            if (parent == nullptr)
                return reductionHappened;
            Node* M = parent->left;
            //Node* N = findApplicant(subTree->right);
            if (M->left != nullptr){
                //resolve all conflicts with alpha-conversion
                alphaConverse(M, subTree->right);
                //substitute all variables bound to M with N
                betaReduce(M, subTree->right);
                //remove lambda expression M
                Node* deletePtr = M;
                parent->left = M->left;
                deletePtr->left = nullptr;
                delete deletePtr;
                //remove applicant N
                delete subTree->right;
                subTree->right = nullptr;
                //Reduction complete! :D
                return true;
            }
        }
        return reductionHappened;
    }

    //Otherwise reduce your left-child
    return reduceSub(subTree->left);
} //SyntaxTree::reduceSub

bool SyntaxTree::betaReduce(Node * M, Node * N)
{
    return betaReduceSub(M->left, N, M->varName);
    
} //SyntaxTree::betaReduce

bool SyntaxTree::betaReduceSub(Node* & subTree, const Node * N, const std::string Mvar)
{
    if (subTree == nullptr)
        return false;
    
    bool result = false; //did we reduce?
    switch (subTree->type) {
    case APPLICATION:
        result = betaReduceSub(subTree->left, N, Mvar);
        result |= betaReduceSub(subTree->right, N, Mvar);
        return result;
    case ABSTRACTION:
        return betaReduceSub(subTree->left, N, Mvar);
    case ATOMIC:
        //TODO: check voor alpha renaming
        if (subTree->varName == Mvar)
        {
            subTree = copy(N);
            return true;
        }
        return betaReduceSub(subTree->left, N, Mvar);
    default:
        return false;
    }
} //SyntaxTree::betaReduce

// UTILITIES //

bool SyntaxTree::containsString(std::vector<std::string> vec, std::string seekStr)
{
    for (size_t i = 0; i < vec.size(); i++) {
        if (vec[i] == seekStr) return true;
    }
    return false;
} //SyntaxTree::containsString

void SyntaxTree::clean()
{
    cleanTree(root);
} //SyntaxTree::clean

void SyntaxTree::cleanTree(Node * & subTree)
{
    if (subTree == nullptr) return;

    switch (subTree->type)
    {
    case APPLICATION:
        cleanTree(subTree->left);
        cleanTree(subTree->right);
        return;
    case ABSTRACTION:
        cleanTree(subTree->left);
        return;
    case ATOMIC:
        if (!subTree->varName.empty()) return;
        if (subTree->left->right == nullptr)
        {
            //Delete the brackets and replace with the contents
            Node* replacement = subTree->left->left;
            subTree->left->left = nullptr;
            delete subTree;
            subTree = replacement;
            cleanTree(subTree);
        }
        else {
            cleanTree(subTree->left);
        }
    }
} //SyntaxTree::cleanTree

Node* SyntaxTree::copy(const Node * copyTree)
{
    if (copyTree == nullptr)
        return nullptr;

    Node* newTree = new Node;
    if (newTree == nullptr)
        throwException(ALLOCATION_ERROR);
    
    //Copy the current Node into the new Node
    newTree->type = copyTree->type;
    newTree->varName = copyTree->varName;

    //Copy the rest of the subTree
    newTree->left = copy(copyTree->left);
    newTree->right = copy(copyTree->right);

    return newTree;
} //SyntaxTree::copyTree

Node* SyntaxTree::findLambdaParent(Node* subTree) const
{
    if (subTree->left != nullptr) {
        if ( subTree->left->type == ABSTRACTION)
            return subTree; //found the parent
        
        return findLambdaParent(subTree->left);
    }
    return nullptr;
} //SyntaxTree::findLambdaParent

Node* SyntaxTree::findApplicant(Node* subTree) const
{
    if (subTree->left->type == ATOMIC && subTree->left->varName.empty()) {
        //Skip a direct brackets
        return subTree->left->left;
    }
    return subTree;
} //SyntaxTree::findLambdaParent

void SyntaxTree::print() const
{
    print(root);
} //SyntaxTree::print


void SyntaxTree::print(Node* node) const
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
    if (root != nullptr) {
        delete root;
    }
} //SyntaxTree::clear
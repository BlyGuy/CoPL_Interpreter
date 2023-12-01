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
    //Parse the judgement
    root->type = JUDGEMENT;
    size_t index = 0;
    bool result = constructParseTreeJudge(tokens, index, root);

    //Checking if parsing finished prematurely
    if (index < tokens.size()) {
        throwException(UNFINISHED_PARSING);
    }
    return result;
} //Syntaxtree::constructParseTree

bool SyntaxTree::constructParseTreeJudge(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    //parse the application    
    subTree->left = new Node;
    if (subTree->left == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    subTree->left->type = APPLICATION;
    if (!constructParseTreeApp(tokens, index, subTree->left)) {
        return false;
    }

    //is there a colon?
    if (tokens[index].type != COLON)
    {
        throwException(MISSING_COLON);
    }
    index++;

    if (index >= tokens.size()) {
        throwException(MISSING_JUDGEMENT_TYPE);
    }
    

    //parse the type
    subTree->right = new Node;
    if (subTree->right == nullptr) {
        throwException(ALLOCATION_ERROR);
    }
    subTree->right->type = TYPE;
    if (!constructParseTreeType(tokens, index, subTree->right)) {
        return false;
    }
    return true;
}

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
    if (index >= tokens.size()
    || tokens[index].type == RIGHT_BRACKET
    || tokens[index].type == COLON) {
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
        if (!(index + 4 < tokens.size() && tokens[index + 1].type == LVAR)) {
            throwException(LAMBDA_MISSING_VAR);
        }
        //Store the Lambda-variable directly in the node
        subTree->varName = tokens[index + 1].varName;

        //is there a superscript?
        if (tokens[index+2].type != SUPERSCRIPT) {
            throwException(MISSING_SUPERSCRIPT);
        }

        //Parse the following Type
        index += 3;
        subTree->left = new Node;
        if (subTree->left == nullptr) {
            throwException(ALLOCATION_ERROR);
        }
        subTree->left->type = TYPE;
        if (!constructParseTreeType(tokens, index, subTree->left)) {
            return false;
        }
        //Parse the following Application
        subTree->right = new Node;
        if (subTree->right == nullptr) {
            throwException(ALLOCATION_ERROR);
        }
        subTree->right->type = ABSTRACTION;
        return constructParseTreeAbstr(tokens, index, subTree->right);
    }

    //Parse the Atomic
    subTree->type = ATOMIC;
    return constructParseTreeAtom(tokens, index, subTree);
} //SyntaxTree::constructParseTreeAbstr


bool SyntaxTree::constructParseTreeAtom(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    switch (tokens[index].type) {
        case LVAR:
            subTree->varName = tokens[index].varName;
            return true;
        case LEFT_BRACKET:
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
        case RIGHT_BRACKET:
            throwException(UNPAIRED_RIGHT_BRACKET);
            break;
        case UVAR:
            std::cerr << "The var: " << tokens[index].varName << std::endl;
            throwException(EXPECTED_LVAR);
            break;
        default:
            break;
    }
    return false;
} //SyntaxTree::constructParseTreeAtom

bool SyntaxTree::constructParseTreeType(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    //Parse the Type
    subTree->left = new Node;
    if (subTree->left == nullptr)
    {
        throwException(ALLOCATION_ERROR);
    }
    subTree->left->type = BASE_TYPE;
    if (!constructParseTreeBType(tokens, index, subTree->left)) {
        return false;
    }

    //is there an Arrow?
    index++;
    if (index < tokens.size() && tokens[index].type == ARROW)
    {   
        index++;
        subTree->right = new Node;
        if (subTree->right == nullptr) {
            throwException(ALLOCATION_ERROR);
        }
        subTree->right->type = TYPE;
        if (!constructParseTreeType(tokens, index, subTree->right)){
            return false;
        }
    } //else {
    //     Node * deletePtr = subTree->left;
    //     subTree->left = copy(subTree->left);
    //     subTree->type = BASE_TYPE
    //     delete deletePtr;
    //     //TODO
    //     //Worth it?
    //     //check this mans genius???
    //     //Optimalisatie?????? -> just purchase better cpu TRUEEEEEEE get gut nerd 🗣️
    // }
    return true; 
}
// vliegveld
//     __|__
// ---o-(_)-o---
//       💥       __
//    __________/ F
//  c'____---__=_/
//___o_____o________
bool SyntaxTree::constructParseTreeBType(std::vector<Token> & tokens, size_t & index, Node* subTree)
{
    switch (tokens[index].type){
        case UVAR:
            subTree->varName = tokens[index].varName;
            return true;
        case LEFT_BRACKET:
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
            subTree->left->type = TYPE;
            if (!constructParseTreeType(tokens, index, subTree->left)){
                return false;
            }

            // Check for a matching right_bracket
            if (tokens[index].type != RIGHT_BRACKET) {
                throwException(UNPAIRED_LEFT_BRACKET);
            }
            return true;
        case RIGHT_BRACKET:
            throwException(UNPAIRED_RIGHT_BRACKET);
            break;
        case LVAR:
            std::cerr << "The var: " << tokens[index].varName << std::endl;
            throwException(EXPECTED_UVAR);
            break;
        default:
            break;
        }
    return false;
}

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
    case JUDGEMENT:
        print(node->left);
        std::cout << ": ";
        print(node->right);
        break;
    case APPLICATION:
        print(node->left);
        print(node->right);
        break;
    case ABSTRACTION:
        std::cout << '\\' << node->varName << " ^ ";
        print(node->left);
        print(node->right);
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
    case TYPE:
        print(node->left);
        if (node->right != nullptr){
            std::cout << "-> ";
            print(node->right);
        }
        break;
    case BASE_TYPE:
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
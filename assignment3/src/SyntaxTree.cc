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
    }
    return true; 
}

bool SyntaxTree::typeCheck()
{
    std::vector<TypeBinding> context;
    Node* exprType = determineType(root->left, context);
    Node* typeToCheck = root->right;
    //Print contents of both
    std::cout << "exprType:    ";
    print(exprType);
    std::cout << std::endl;
    std::cout << "typeToCheck: ";
    print(typeToCheck);
    std::cout << std::endl;
    bool result = checkTypeEquivalence(exprType, typeToCheck);
    if (exprType != nullptr) {
        delete exprType;
    }
    return result;
} //SyntaxTree::typeCheck

bool SyntaxTree::checkTypeEquivalence(const Node* type1, const Node* type2) const
{
    //check if same pointer was passed in
    if (type1 == type2)
        return true;
    
    //if both would be a nullptr the first test would've passed
    if (type1 == nullptr || type2 == nullptr)
        return false;
    
    //If both are a BASE_TYPE
    if (type1->type == BASE_TYPE && type2->type == BASE_TYPE)
    {
        //Bracket-equivalence
        if (type1->varName.empty() && type2->varName.empty())
            return checkTypeEquivalence(type1->left, type2->left);
        //If one var, check for var in brackets
        if (type1->varName.empty() || type2->varName.empty()) {
            return type1->varName.empty() ?
                checkTypeEquivalence(type1->left->left, type2) :
                checkTypeEquivalence(type1, type2->left->left);
        }
        
        //Var-equivalence
        return type1->varName == type2->varName;
    }

    //if one is TYPE and other is BASE_TYPE
    if (type1->type != type2->type)
    {
        return type1->type == BASE_TYPE ?
            checkTypeEquivalence(type1, type2->left) :
            checkTypeEquivalence(type1->left, type2);
    }
    
    //Check if every node in both subTree's is the same as the other tree
    return  (type1->type == type2->type)  /* both TYPE */    &&
            checkTypeEquivalence(type1->left, type2->left) &&
            checkTypeEquivalence(type1->right, type2->right);
} //SyntaxTree::checkTypeEquivalence

Node* SyntaxTree::determineType(const Node* subTree, std::vector<TypeBinding> & context)
{
    Node* leftType = nullptr;
    Node* rightType = nullptr;
    Node* returnType = nullptr;
    Node* childType = nullptr;
    switch (subTree->type)
    {
    case APPLICATION:
        leftType = determineType(subTree->left, context);
        //Check for a standalone lambda expression
        if (subTree->right == nullptr) {
            return leftType;
        }
        //otherwise, check if the two types can be applied
        rightType = determineType(subTree->right, context);
        //Find parent of the final return type
        returnType = leftType;
        while (returnType != nullptr && returnType->right == nullptr) {
            returnType = returnType->left;
        }
        if (returnType == nullptr)
            throwException(APPLICATION_HAS_NO_RETURNTYPE);
        
        std::cout << "leftType:  ";
        print(returnType->left);
        std::cout << std::endl;
        std::cout << "rightType: ";
        print(rightType);
        std::cout << std::endl;
        childType = rightType;
        while (childType != nullptr && !checkTypeEquivalence(returnType->left, childType)) {
            //go one brackets deeper
            if (childType->left != nullptr && childType->left->type == BASE_TYPE) {
                childType = childType->left->left;
            } else {
                childType = nullptr;
            }            
        }
        if (childType == nullptr) { //No equivalent rightType found
            throwException(APPLICATION_UNRESOLVABLE);
        }

        std::cout << "returnType:  ";
        print(returnType->right);
        std::cout << std::endl;
        //Free all unnecessary type-nodes
        if (rightType != nullptr)
            delete rightType;
        if (leftType != nullptr) {
            childType = returnType;
            returnType = returnType->right;
            childType->right = nullptr;
            delete leftType;
        }
        //and return the resulting type
        return returnType;
    case ABSTRACTION:
        //Add the type-declaration of the abstraction to the context
        context.push_back({subTree->varName, subTree->left});
        childType = determineType(subTree->right, context);

        //Allocate new nodes to add onto the returned child-type
        returnType = copy(subTree->left);
        rightType = returnType;
        //Get the rightmost node for return type placement
        while (rightType->right != nullptr) {
            rightType = rightType->right;
        }
        rightType->right = childType;
        //Remove our type-declaration from context 
        context.pop_back();
        return returnType;
    case ATOMIC:
        if (subTree->varName.empty()) {
            //type-check for brackets
            childType = determineType(subTree->left, context);
            //Allocate Nodes for a bracketed type
            returnType = new Node;
            if (returnType == nullptr)
                throwException(ALLOCATION_ERROR);
            returnType->type = TYPE;
            returnType->left = new Node;
            if (returnType->left == nullptr)
                throwException(ALLOCATION_ERROR);
            returnType->left->type = BASE_TYPE;
            returnType->left->left = childType;
            //return bracketed childtype
            return returnType;
            
        }
        //type-check for var
        //checks through the most recent bindings in context first
        for (size_t i = context.size(); i-- > 0;) {
            if (context[i].var == subTree->varName) {
                return copy(context[i].type);
            }
        }
        throwException(UNDECLARED_TYPE);
        break;
    default:
        return nullptr;
    }
    return nullptr;
} //SyntaxTree::determineType

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

Node* SyntaxTree::copy(Node * copyTree) const
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
        //std::cout << "type: ";
        print(node->left);
        if (node->right != nullptr){
            std::cout << "-> ";
            print(node->right);
        }
        break;
    case BASE_TYPE:
        //std::cout << "bType: ";
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
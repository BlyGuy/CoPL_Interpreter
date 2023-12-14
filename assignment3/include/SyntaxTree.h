/**
 * @file SyntaxTree.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief Header file for the SyntaxTree class.
 *        Functions as a syntactical analyser of lambda expressions
 */
#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <iostream>
#include <vector>
#include "Lexer.h"
#include "errors.h"

/**
 * Ambiguous grammar definition:
 * 
 * <Judgement>      ::= <Application> COLON <Type>
 *
 * <Application>    ::= <Abstraction> <Application> 
 *                    | <Abstraction>
 * <Abstraction>    ::= LAMBDA LVAR SUPERSCRIPT <Type> <Application>
 *                    | <Atomic>
 * <Atomic>         ::= LEFT_BRACKET <Application> RIGHT_BRACKET
 *                    | LVAR
 *
 * <Type>           ::= <Base Type> ARROW <Type>
 *                    | <BaseType>
 * <BaseType>       ::= LEFT_BRACKET <Type> RIGHT_BRACKET
 *                    | UVAR 
 *                    
 * First sets:
 * First (<BaseType>) = {UVAR, LEFT_BRACKET)})
 * First(<Type>) = {First(Basetype)}
 * 
 * First(<Atomic>) = {LVAR, LEFT_BRACKET}
 * First(<Abstraction>) = {LAMBDA, LVAR, LEFT_BRACKET}
 * First(<Application>) = {LVAR, LEFT_BRACKET, LAMBDA}
 * First(<Judgement>) = First{Application}
 */

enum ENodeType {
    JUDGEMENT,
    APPLICATION,
    ABSTRACTION,
    ATOMIC,
    TYPE,
    BASE_TYPE
};



class Node {
public:
    ENodeType type;
    std::string varName = "";
    Node* left = nullptr;
    Node* right = nullptr;

    ~Node(); //destructor
};

struct TypeBinding
{
    std::string var = "";
    Node* type = nullptr;
};

class SyntaxTree
{
private:
    Node* root; //root node of the parse-tree

    /**
     * @brief constructs the judgement from the parsetree recursively
     *        and stores the corresponding tokens
     * 
     * @param tokens    the vector to store tokens
     * @param index     the index of the look-ahead token
     * @param subTree   the tree in question
     * @return true 
     * @return false 
     */
    bool constructParseTreeJudge(std::vector<Token> & tokens, size_t & index, Node* subTree);

    /**
    * @brief constructs the application from the parsetree recursively
    *        and stores the corresponding tokens
    * 
    * @param tokens    the vector to store tokens
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the application is valid and has been succesfully
                        represented in the parse tree
    * @return false    the application is invalid
    */
    bool constructParseTreeApp(std::vector<Token> & tokens, size_t & index, Node* subTree);

    
    /**
    * @brief constructs the abstraction from the parsetree recursively
    *        and stores the corresponding tokens
    * 
    * @param tokens    the vector to store tokens
    * @param index     the given index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the abstraction is valid and has been succesfully
                        represented in the parse tree
    * @return false    the abstraction is invalid
    */
    bool constructParseTreeAbstr(std::vector<Token> & tokens, size_t & index, Node* subTree);
    
    /**
    * @brief constructs the atom from the parsetree recursively
    *        with the given information from the lexical analyzer    
    *        and stores the corresponding tokens
    * 
    * @param tokens    the vector to store tokens
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the atom is valid and has been succesfully
                       represented in the parse tree
    * @return false    the atom is invalid
    */    
    bool constructParseTreeAtom(std::vector<Token> & tokens, size_t & index, Node* subTree);

    /**
    * @brief constructs the type from the parsetree recursively
    *        with the given information from the lexical analyzer    
    *        and stores the corresponding tokens
    * 
    * @param tokens    the vector to store tokens
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the type is valid and has been succesfully
                       represented in the parse tree
    * @return false    the type is invalid
    */  
    bool constructParseTreeType(std::vector<Token> & tokens, size_t & index, Node* subTree);

    /**
    * @brief constructs the base type from the parsetree recursively
    *        with the given information from the lexical analyzer    
    *        and stores the corresponding tokens
    * 
    * @param tokens    the vector to store tokens
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the base type is valid and has been succesfully
                       represented in the parse tree
    * @return false    the base type is invalid
    */  
    bool constructParseTreeBType(std::vector<Token> & tokens, size_t & index, Node* subTree);


    /**
     * @brief determines the type of the variable in the subtree
     *        and adds the judgement to the context
     * 
     * @param subTree the subtree in question
     * @param context the context to store judgements in
     * @return Node* the node pointing to the resulting type
     */
    Node* determineType(const Node* subTree, std::vector<TypeBinding> & context);

    /**
     * @brief Copies the contents of copyTree to the destination Node
     * 
     * @param copyTree The subtree that gets copied
     * @return A pointer to the copied subtree
     */
    Node* copy(const Node * copyTree);

    /**
    * @brief prints the syntax tree recursively
    * 
    * @param node the current node
    */
    void print(Node* node);

public:
    SyntaxTree();
    SyntaxTree(Lexer & lex);
    ~SyntaxTree();

    /**
    * @brief constructs the parse tree with the given information
            from the lexical analyzer
    * 
    * @param lex       the lexical analyzer in question
    * @return true     the expression was valid and has
    *                  been succesfully represented in a parse tree
    * @return false    the expression is invalid
    *                  or the parse tree couldn't be created
    */
    bool constructParseTree(std::vector<Token> & tokens);

    /**
     * @brief Checks if the type after the colon matches the type of the preceding expression.
     * 
     * @return true: if yes.
     * @return false: if no.
     */
    bool typeCheck();

    /**
     * @brief 
     * 
     * @param type1 
     * @param type2 
     * @return true 
     * @return false 
     */
    bool checkTypeEquivalence(const Node* type1, const Node* type2) const;
    
    /**
     * @brief prints the syntax tree
     */
    void print();

    /**
    * @brief clears the syntax tree
    */
    void clear();
};


#endif //SYNTAX_TREE_H
/**
 * @file SyntaxTree.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author N.I. van Laarhoven
 * @brief Header file for the SyntaxTree class.
 *        Functions as a syntactical analyser
 *        and a resolver of lambda expressions
 * @date ????
 */
#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <iostream>
#include <vector>
#include <string>
#include "Lexer.h"
#include "errors.h"
#include "constants.h"

/**
 * Unambiguous grammar definition:
 * 
 * <Application>    ::= <Abstraction> <Application> 
 *                    | <Abstraction>
 * <Abstraction>    ::= LAMBDA VAR <Abstraction>
 *                    | <Atomic>
 * <Atomic>         ::= LEFT_BRACKET <Application> RIGHT_BRACKET
 *                    | VAR
 * First(<Atomic>) = {LEFT_BRACKET, VAR}
 * First(<Abstraction>) = {LEFT_BRACKET, VAR, LAMBDA}
 * First(<Application>) = First{Abstraction}
 */

enum ENodeType {
    APPLICATION,
    ABSTRACTION,
    ATOMIC
};

class Node {
public:
    ENodeType type;
    std::string varName = "";
    Node* left = nullptr;
    Node* right = nullptr;

    ~Node();
};

class SyntaxTree
{
private:
    Node* root; //root node van de parse-tree
    
    /**
    * @brief constructs the application from the parsetree recursively
    *        with a given token-string   
    *
    * @param tokens    the tokens in question
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the application is valid and has been succesfully
                        represented in the parse tree
    * @return false    the application is invalid
    */
    bool constructParseTreeApp(std::vector<Token> & tokens, size_t & index, Node* subTree);
    
    /**
    * @brief constructs the abstraction from the parsetree recursively
    *        with a given token-string    
    *
    * @param tokens    the tokens in question
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the abstraction is valid and has been succesfully
                        represented in the parse tree
    * @return false    the abstraction is invalid
    */
    bool constructParseTreeAbstr(std::vector<Token> & tokens, size_t & index, Node* subTree);
    
    /**
    * @brief constructs the atom from the parsetree recursively
    *        with the given token-string    
    *
    * @param tokens    the tokens in question
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the atom is valid and has been succesfully
                        represented in the parse tree
    * @return false    the atom is invalid
    */    
    bool constructParseTreeAtom(std::vector<Token> & tokens, size_t & index, Node* subTree);

    /**
     * @brief search first parent of a lambda expression recursively in the subtree,
     *        left precedence is applied
     * 
     * @param subtree the subtree to be searched
     * @return Node* to the parent of a lambda expression
     */
    Node* findLambdaParent(Node* subtree);

    /**
     * @brief performs beta-reduction on M with N
     * 
     * @param M The lambda abstraction that N is applied to
     * @param N The subtree that needs to be applied to M
     * @pre M and N are not NULL-pointers (real subtree)
     * @return true TODO
     * @return false TODO
     */
    bool betaReduce(Node* M, Node* N);

    /**
     * @brief actually performs the beta reduction, (λyM)N -> M[x:=N]
     * 
     * @param M TODO
     * @param N TODO
     * @param Mvar TODO
     * @return true 
     * @return false 
     */
    bool betaReduceSub(Node* & subTree, const Node * N, const std::string Mvar);
    
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
    * @brief constructs the parse tree with a given token-string
    * 
    * @param tokens    the tokens
    * @return true     the expression was valid and has
    *                  been succesfully represented in a parse tree
    * @return false    the expression is invalid
    *                  or the parse tree couldn't be created
    */
    bool constructParseTree(std::vector<Token> & tokens);


    /**
     * @brief converses the expression, λyM[x:=y]
     * 
     * @param subTree the root of the expression, M
     * @param var the variable to be substituted, x
     * @param subs the substitute variable, y
     * @return true the expression has been succesfully conversed
     * @return false the expression has not been conversed
     */
    bool converse(Node* subTree, Token & var, std::string subs);

    /**
     * @brief Performs beta-reduction and alpha-renaming on the syntaxtree
     *        until it is no longer possible or a reduction limit has been reached
     * @return true No reductions possible, expression fully reduced
     * @return false Reductions still possible, reduction limit reached
     */
    bool reduce();

    /**
     * @brief beta reduces the expression, M[x:=N]
     * 
     * @param subTreeSubst the substitute expression, N
     * @param subTreeExpr the expression to be reduced, M
     * @param subTreeVar the variable to be substituted
     * @param subTreeRoot the root of the expression
     * @return true the expression has been succesfully reduced
     * @return false the expression has not been reduced
     */
    bool reduceSub(Node* subTree);


    /**
     * @brief prints the syntax tree
     * 
     */
    void print();

    /**
    * @brief clears the syntax tree
    * 
    */
    void clear();
};


#endif //SYNTAX_TREE_H
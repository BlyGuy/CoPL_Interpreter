/**
 * @file SyntaxTree.h
 * @author Marc Hoek
 * @author Joshua J. Lelipaly
 * @author Noah I. van Laarhoven
 * @brief Header file for the SyntaxTree class.
 *        Functions as a syntactical analyser
 *        and a resolver of lambda expressions
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
 * <Expr>           ::= <Application>
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

/**
 * @brief enum for categorizing the types of nodes
 * 
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

    ~Node(); //destructor
};

class SyntaxTree
{
private:
    Node* root; //root node of the parse-tree
    
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

    // BETA-REDUCTION FUNCTIONS //

    /**
     * @brief search the first parent of a lambda expression recursively
     *        in the subtree, left precedence is applied
     * 
     * @param subtree the subtree to be searched
     * @return Node* to the parent of a lambda expression
     */
    Node* findLambdaParent(Node* subtree) const;

    /**
     * @brief search first parent of a lambda expression recursively in the subtree,
     *        left precedence is applied
     * 
     * @param subtree the subtree to be searched
     * @return Node* to the parent of application N
     */
    Node* findApplicant(Node* subtree) const;

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
    
    // ALPHA-CONVERSION FUNCTIONS //
    
    /**
     * @brief Checks for beta-reduction conflicts
     *        and performs alpha-conversion on the conflicting variables
     * 
     * @param M the root of the lambda-expression, M
     * @param N the root of the applicant, N
     */
    void alphaConverse(Node* M, const Node* N);
    
    /**
     * @brief Get all the bound variables in an expression-tree
     *        added to the result vector
     * 
     * @param result the resulting vector with all bound vars added
     * @param subTree the expression in which to look for bound vars
     */
    void getBoundVars(std::vector<std::string> & result, const Node* subTree);

    /**
     * @brief Get the conflicting variables in an expression-tree
     *        added to the result vector
     * 
     * @param result the resulting vector with all conflicting vars added
     * @param Mbound all variables bound in lambda expression M
     * @param Nbound all variables bound in applicant expression N
     * @param subTree the expression in which to look for conflicting vars
     */
    void getConflictVars(std::vector<std::string> & result, const std::vector<std::string> Mbound, const std::vector<std::string> Nbound, const Node* subTree);
    
    /**
     * @brief Performs alpha-renaming on all conflicting variables in a subTree
     * 
     * @param conflicts all conflicting variables
     * @param Mbound all variables bound in lambda expression M
     * @param subTree the expression in which to resolve conflicts
     */
    void resolveConflicts(const std::vector<std::string> & conflicts, const std::vector<std::string> Mbound, Node* subTree);
    
    // UTILITIES //

    /**
     * @brief Checks if a variable-vector contains a given variable
     * 
     * @param vec The vector to search in
     * @param seekVar The variable that will be searched for
     * @return true: vec contains seekVar.
     * @return false: vec doesn't contain seekVar.
     */
    bool containsString(std::vector<std::string> vec, std::string seekStr);

    /**
     * @brief Copies the contents of copyTree to the destination Node
     * 
     * @param copyTree The subtree that gets copied
     * @return A pointer to the copied subtree
     */
    Node* copy(const Node * copyTree);

    /**
     * @brief Clean the subTree of unneccesary brackets.
     *        This will remove all ATOMIC nodes with an application with only 1 child
     * 
     * @param subTree The subTree to remove unneccessary brackets from
     */
    void cleanTree(Node * & subTree);

    /**
     * @brief cleans the SyntaxTree of unneccesary brackets.
     *        This will remove all ATOMIC nodes with an application with only 1 child
     */
    void clean();

    /**
    * @brief prints the syntax tree recursively
    * 
    * @param node the current node
    */
    void print(Node* node) const;

public:
    SyntaxTree();
    SyntaxTree(Lexer & lex);
    ~SyntaxTree();

    /**
    * @brief constructs the parse tree with a given token-string
    * 
    * @param tokens    the tokens
    * @return true:    The expression was valid and has
    *                  been succesfully represented in a parse tree.
    * @return false:   The expression is invalid
    *                  or the parse tree couldn't be created.
    */
    bool constructParseTree(std::vector<Token> & tokens);

    /**
     * @brief Performs beta-reduction and alpha-renaming on the syntaxtree
     *        until it is no longer possible or a reduction limit has been reached
     * @return true: No reductions possible, expression fully reduced.
     * @return false: Reductions still possible, reduction limit reached.
     */
    bool reduce();

    /**
     * @brief looks if beta reductions are possible in a subtree and performs them if possible
     * 
     * @param subTree the tree to be searched for beta-reductions
     * @return true: the expression has been succesfully reduced.
     * @return false: the expression has not been reduced.
     */
    bool reduceSub(Node* subTree);


    /**
     * @brief prints the syntax tree
     */
    void print() const;

    /**
    * @brief clears/empties the syntax tree
    */
    void clear();
};

#endif //SYNTAX_TREE_H
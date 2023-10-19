#ifndef SYNTAX_TREE_H
#define SYNTAX_TREE_H

#include <iostream>
#include <vector>
#include <climits>
#include "Lexer.h"
#include "SymTable.h"
#include "errors.h"

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
    size_t symTableID = ULONG_MAX;
    Node* left = nullptr;
    Node* right = nullptr;

    ~Node();
};

class SyntaxTree
{
private:
    Node* root; //root node van de parse-tree
    std::vector<SymTableEntry> symTable; //SymTableEntry vector that contains variable information
    
    /**
    * @brief constructs the application from the parsetree recursively
    *        with the given information from the lexical analyzer     
    *
    * @param lex       the lexical analyzer in question
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the application is valid and has been succesfully
                        represented in the parse tree
    * @return false    the application is invalid
    */
    bool constructParseTreeApp(std::vector<Token> & tokens, size_t & index, size_t & symTableIndex, Node* subTree);

    
    /**
    * @brief constructs the abstraction from the parsetree recursively
    *        with the given information from the lexical analyzer    
    *
    * @param lex       the lexical analyzer in question
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the abstraction is valid and has been succesfully
                        represented in the parse tree
    * @return false    the abstraction is invalid
    */
    bool constructParseTreeAbstr(std::vector<Token> & tokens, size_t & index, size_t & symTableIndex, Node* subTree);
    
    /**
    * @brief constructs the atom from the parsetree recursively
    *        with the given information from the lexical analyzer    
    *
    * @param lex       the lexical analyzer in question
    * @param index     the index of the look-ahead token
    * @param subTree   the tree in question
    * @return true     the atom is valid and has been succesfully
                        represented in the parse tree
    * @return false    the atom is invalid
    */    
    bool constructParseTreeAtom(std::vector<Token> & tokens, size_t & index, size_t & symTableIndex, Node* subTree);

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

    bool converse(Node* subTree, Token & var, std::string subs);

    bool reduce(Node* subTreeSubst, Node* subTreeExpr, Node* subTreeVar, Node* subTreeRoot);

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
#ifndef __PARSER_HPP__
#define __PARSER_HPP__

#include"arith-scanner.hpp"
#include"arith-ast.hpp"

class Parser{
  private:
    Scanner *_scanner; // Scanner instance
    AstNode *_goal; // root node of the input
    Token _token; // current token in the scanner
    bool _error; // indicates any "incorrect syntax" error

    void setError() { _error = true; };
    bool readToken(enumToken type, Token *token = NULL); // detect the given type of token
    AstNode* goal(); // takes entire goal and make AST node
    AstExpression* expression(); // call exspression routine. expr -> term expr' If expressionP returns NULL, it means it is not a AstBinaryOp* object, so return AstExpression* object from term. If it is not a NULL, dynamic_cast that one and return AstBinaryOp* object.
    AstBinaryOp* expressionP(); // call expression' routine. expr' -> {'+' | '-'} expr' | eps. If it matches with eps, return NULL
    AstExpression* term(); // call term routine. term -> factor term'. If termP returns NULL, return AstOperand* object from factor. If it is not a NULL, dynamic_cast that one and return AstBinaryOp* object.
    AstBinaryOp* termP(); // call term' routine. term' -> {'*' | '/'} term' | eps. If it matches with eps, return NULL
    AstOperand* factor(); // call factor routine. factor -> num | ID.
    AstIdent* ident(); // call ID routine. ID -> alphabet.
    AstConstant* constant(); // call number routine. number -> {digit}+.
  public:
    Parser(Scanner *scanner); // accepts scanner immediately
    AstNode* parse(); // starts parsing
    bool hasError() const { return _error; };
};

#endif

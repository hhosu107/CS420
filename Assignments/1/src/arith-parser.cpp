#include<cassert>
#include<cstdlib>
#include<vector>
#include<stack>
#include<iostream>

#include"arith-parser.hpp"
using namespace std;

// Parser initialization
Parser::Parser(Scanner *scanner){
  _scanner = scanner;
  _goal = NULL;
}

AstNode* Parser::parse(){
  _error = false;
  if(_goal != NULL){
    delete _goal; _goal = NULL;
  }
  try{
    if(_scanner != NULL) _goal = goal();
    if(_goal != NULL){

    }
  } catch (...) {
    _goal = NULL;
  }
  return _goal;
}

bool Parser::readToken(enumToken type, Token *token){
  // if there is already an error, stop and return false
  if(_error) return false;

  Token t = _scanner->getNext(); // get token and prepare scanner to send next token
  if(t.getTokenType() != type){
    setError();
  }
  if(token != NULL) *token = t;
  return t.getTokenType() == type;
}

AstNode* Parser::goal(){
  readToken(tStart); // Initial virtual token.
  AstNode *_goal = expression();
  if(hasError()){ // Skip all others.
    return NULL;
  } else if( _scanner->peekNext().getTokenType() != tEOF){ // In some cases, expression() might return NULL without calling setError().
    setError();
    return NULL;
  }
  else return _goal;
}

AstExpression* Parser::expression(){
  AstExpression* _term = term();
  AstBinaryOp* _exprp = NULL;
  if(hasError()){ // Skip all others.
    return NULL;
  }
  else{
    _exprp = expressionP();
    if(hasError()){
      return NULL;
    }
    if(_exprp != NULL){ // _exprp might be empty.
      Operation _opp = _exprp->getOperation();
      AstExpression* _rightp = _exprp->getRight();
      Token _tp = _exprp->getToken();
      return new AstBinaryOp(_tp, _opp, _term, _rightp);
    }
    else{
      return _term;
    }
  }
}

AstBinaryOp* Parser::expressionP(){
  Token token;
  Operation binOp;
  AstExpression *right = NULL;
  if(_scanner->peekNext().getTokenType() == tPlusMinus){
    readToken(tPlusMinus, &token); // take "+" or "-"
    right = expression(); // Then expression() shouldn't be empty.
    if(hasError() || right == NULL){
      setError();
      return NULL;
    }
    else{
      if(token.getTokenValue() == "+") binOp = operationAdd;
      else if(token.getTokenValue() == "-") binOp = operationSub;
      return new AstBinaryOp(token, binOp, right);
    }
  }
  else return NULL;
}

AstExpression* Parser::term(){
  AstExpression* _factor = factor();
  AstBinaryOp* _termp = NULL;
  if(hasError()){ // skip all others
    return NULL;
  }
  else{
    _termp = termP();
    if(hasError()){
      return NULL;
    }
    if(_termp != NULL){
      Operation _opp = _termp->getOperation();
      AstExpression* _rightp = _termp->getRight();
      Token _tp = _termp->getToken();
      return new AstBinaryOp(_tp, _opp, _factor, _rightp);
    }
    else{
      return _factor;
    }
  }
}

AstBinaryOp* Parser::termP(){
  Token token;
  Operation binOp;
  AstExpression *right = NULL;
  if(_scanner->peekNext().getTokenType() == tMulDiv){
    readToken(tMulDiv, &token);
    right = term(); // then right should not be empty.
    if(hasError() || right == NULL){
      setError();
      return NULL;
    }
    else{
      if(token.getTokenValue() == "*") binOp = operationMul;
      else if(token.getTokenValue() == "/") binOp = operationDiv;
      return new AstBinaryOp(token, binOp, right);
    }
  }
  else return NULL;
}

AstOperand* Parser::factor(){
  Token token;
  if(_scanner->peekNext().getTokenType() == tID){
    return ident();
  }
  else if(_scanner->peekNext().getTokenType() == tNumber){
    return constant();
  }
  else{
    setError();
    return NULL;
  }
}

AstIdent* Parser::ident(){ // cannot make error without semantic-checking
  Token token;
  readToken(tID, &token);
  return new AstIdent(token, token.getTokenValue());
}

AstConstant* Parser::constant(){ // cannot make error without semantic-checking
  Token token;
  readToken(tNumber, &token);
  return new AstConstant(token, token.getTokenValue());
}

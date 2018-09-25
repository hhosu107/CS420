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

CAstNode* Parser::parse(){
  _error = false;
  if(_goal != NULL){
    delete _goal; _goal = NULL;
  }
  try{
    if(_scanner != NULL) _goal = goal();
    if(_goal != NULL){

    }
  } catch {
    _goal = NULL;
  }
  return _goal;
}

bool Parser::readToken(enumToken type, Token *token){
  // if there is already an error, stop and return false
  if(_error) return false;

  Token t = _scanner->getNext();
  if(t.getTokenType() != type){
    setError();

  }
  if(token != NULL) *token = t;
  return t.getTokenType() == type;
}


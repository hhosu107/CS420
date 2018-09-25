#include<cstdlib>
#include<iostream>
#include<sstream>
#include<cctype>
#include<cstring>
#include<string>
#include<cassert>
#include<cstdio>

#include "arith-scanner.hpp"
using namespace std;

#define TOKEN_LEN 12

char enumTokenName[][TOKEN_LEN] = {
  "tNumber",
  "tID",
  "tPlusMinus",
  "tMulDiv",
  "tEOF",
  "tError",
  "tUndefined"
};

Token::Token(enumToken type, const stringvalue){
  _type = type;
  _value = value;
}

Token::Token(const Token &token){
  _type = token.getTokenType();
  _value = token.getTokenValue();
}

Token::Token(const Token *token){
  _type = token->getTokenType();
  _value = token->getTokenValue();
}

static const string Token::tokenName(enumToken type){
  return string(enumTokenName[type]);
}

const string Token::getTokenName(){
  return string(enumTokenName[getTokenType()]);
}

ostream& Token:print(ostream &out) const{
  out << getTokenValue();
  return out;
}

ostream& operator<<(ostream &out, const Token &t){
  return t.print(out);
}

ostream& operator<<(ostream &out, const Token *t){
  return t->print(out);
}

Scanner::Scanner(istream *in){
  _in = in;
  _delete = false;
  _okay = true;
  _token = NULL;
}

Scanner::Scanner(string in){
  _in = new istringstream(in);
  _delete = false;
  _okay = true;
  _token = NULL;
}

Scanner::~Scanner(){
  if(_token != NULL) delete _token;
  if(_delete) delete _in;
}

Token Scanner::getNext(){
  Token result(_token); // prepare the next token: note that this initializer copies the content, so changing _token doesn't affect.

  enumToken type = _token->getTokenType();
  _okay = !(type == tError); // check the current token: tError or not

  nextToken(); // and get the next token and return itself
  return result;
}

Token Scanner::peekNext() const{
  return Token(_token);
}

void Scanner::nextToken(){
  if(_token != NULL) delete _token;
  _token = scan(); // fill in the next token
}

Token* Scanner::scan(){
  // Rule-based scan
}

Token* Scanner::newToken(enumToken token, const string value){
  return new Token(token, value);
}

Token* Scanner::scan(){
  enumToken token;
  string value;
  char c;

  while(_in->good() && isWhite(_in->peek())){
    getNextChar();
  } // eliminate whitespaces. Since it peeks, the next getNextChar() will return the valid character.
  // stringstream states: good / eof / fail / bad. Any false value of good() except eof means an error on reading character on the stringstream.
  if(_in->eof()) return newToken(tEOF); // EOF token
  if(!_in->good()) return newToken(tError); // error on scanning

  c = getNextChar();
  
  // initialize members of token
  value = c;
  token = tUndefined;

  // tokenize by rule: digit / alphabet / others
  switch(c){
    case '+':
    case '-':
      token = tPlusMinus;
      break;
    case '*':
    case '/':
      token = tMulDiv;
      break;
    default:
      if(isDigit(c)){
        token = tNumber;
        // peek until characters other than digit found
        c = _in->peek();
        while(!isDigit(c)){
          value += getNextChar();
          c = _in->peek();
        }
      }
      else if(isAlphabet(c)){
        token = tID;
      }
      else{ // since whitespaces are already detected before switch starts, other characters are invalid.
        value = "";
        token = tError;
        _okay = false; 
      }
      break;

  }

  return new Token(token, value);
}

char Scanner::getNextChar(){
  return _in->get();
}

bool isDigit(char c){
  return (c >= '0' && c <= '9');
}

bool isAlphabet(char c){
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isWhite(char c){
  return (c == ' ' || c == '\n' || c == '\t' || c == 'r');
}

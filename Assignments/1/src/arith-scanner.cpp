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
  "tStart",
  "tEOF",
  "tError",
  "tUndefined"
};

Token::Token(){
  _type = tUndefined;
  _value = "";
}

Token::Token(enumToken type, const string value){
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

const string Token::tokenName(enumToken type){
  return string(enumTokenName[type]);
}

const string Token::getTokenName() const{
  return string(enumTokenName[getTokenType()]);
}

ostream& Token::print(ostream &out) const{
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
  _token = new Token(tStart, ""); // Initialize with dummy token indicating start (expression doesn't contain any initializing symbol)
}

Scanner::Scanner(string in){
  _in = new istringstream(in);
  _delete = false;
  _okay = true;
  _token = new Token(tStart, ""); // Initialize with dummy token indicating start (expression doesn't contain any initializing symbol)
}

Scanner::~Scanner(){
  if(_token != NULL) delete _token;
  if(_delete) delete _in;
}

Token Scanner::getNext(){
  Token result(_token); // prepare the current token so that parser recognizes next token

  enumToken type = _token->getTokenType();
  _okay = !(type == tError); // check the current token: tError or not

  nextToken(); // and prepare the next token in scanner
  return result;
}

Token Scanner::peekNext() const{ // return current token to parser so that parser recognizes next token
  return Token(_token);
}

void Scanner::nextToken(){
  if(_token != NULL) delete _token;
  _token = scan(); // fill in the next token
}

Token* Scanner::newToken(enumToken token){
  return new Token(token);
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
        while(isDigit(c)){
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

bool Scanner::isDigit(char c) const{
  return (c >= '0' && c <= '9');
}

bool Scanner::isAlphabet(char c) const{
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool Scanner::isWhite(char c) const{
  return (c == ' ' || c == '\n' || c == '\t' || c == 'r');
}

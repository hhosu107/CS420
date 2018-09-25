#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__

#include<iostream>
#include<iomanip>
#include<map>
#include<vector>
#include<algorithm>
#include<cstring>
#include<string>
#include<cstdlib>

using namespace std;

enum enumToken{
  tNumber = 0, // Integer := digit { digit }
  tID, // Identifier := alphabet
  tPlusMinus, // PlusMinus := '+' | '-'
  tMulDiv, // MulDiv := '*' | '/'
  tEOF, // Indicating the end of the line.
  tError, // any error on the scanner: character is not a value included in digit or alphabet.
  tUndefined // Initial token.
};

class Token{
  friend class Scanner;
  private:
    enumToken _type; // contains the type of the token
    string _value; // contains the value of the token
  public:
    Token(enumToken type, const string value="");
    Token(const Token &token); // copy token from the reference
    Token(const Token *token); // copy token from the pointer
    static const string tokenName(enumToken type); // return the name of the token type for given type
    const string getTokenName() const; // return the name of the token type of the given token
    enumToken getTokenType() const{
      return _type;
    } // return the enumerated type of the token
    string getTokenValue() const{
      return _value;
    } // return the value of the token
    ostream& print(ostream &out) const; // print the value of the token to the output stream
};

ostream& operator<<(ostream &out, const Token &t);

ostream& operator<<(ostream &out, const Token *t);

class Scanner{
  private:
    istream * _in; // input stream
    bool _delete; // delete inpus stream when destruct
    bool _okay; // Does scanner has no error or not?
    Token * _token; // Contains the next token in the input stream 
    // vector<Token*> _token // vector of Token pointers from the input stream
  public:
    Scanner(istream *in); // Use stream operators like peek
    Scanner(string in); // Change string to string stream
    ~Scanner(); // destruct

    Token getNext(); // return and remove the next token
    Token peekNext() const; // peek the next token without removing it
    bool okay() const { return _okay; }; // Check the status of the scanner
    void nextToken(); // scan the next token
    Token* newToken(enumToken type, const string token); // initializing token by given information
    Token* scan(); // scan the input stream and return the next token
    char getNextChar(); // next character in the input stream
    bool isDigit(char c) const; // check if a character is a digit = ('0', ..., '9')
    bool isAlphabet(char c) const; // check if a character is an alphabet = ('A', ..., 'Z', 'a', ..., 'z')
    bool isWhite(char c) const; // check if a character is a whitespace character = (' ', '\n', 't')
};
#endif

#ifndef __AST_HPP__
#define __AST_HPP__

#include<iostream>
#include<sstream>
#include<iomanip>
#include<map>
#include<vector>
#include"arith-scanner.hpp"

#define OPERATION_LEN 8

using namespace std;

class AstExpression;

enum Operation{
  operationAdd = 0,
  operationSub,
  operationMul,
  operationDiv
};

ostream& operator<<(ostream &out, Operation t);

// AST of arithmetic expression needs these type of nodes:
// 1. Binary operation node: contains operator itself, contains left/right operand (one of three: binary operation node, identifier node, constand node). Nonterminal.
// 2. Identifier node: contains a name of identifier. Terminal.
// 3. Constant node: contains a multi-digit string. Terminal.
class AstNode{ // base node for all node types in the AST
  private:
    Token _token; // token in input stream that triggered this node
    int _id; // Id of the node itself
    static int _global_id; // holds the next ID
  public:
    AstNode(Token token);
    virtual ~AstNode();

    int getID() const; // return the ID of this node
    Token getToken() const; // return the token associated with this node
    virtual ostream& print(ostream &out) const = 0;

};

ostream& operator<<(ostream &out, const AstNode &t);
ostream& operator<<(ostream &out, const AstNode *t);

class AstExpression : public AstNode{
  public:
    AstExpression(Token token);
};

class AstBinaryOp : public AstExpression{
  private:
    Operation _op;
    AstExpression *_left;
    AstExpression *_right;
  public:
    AstBinaryOp(Token token, Operation op, AstExpression *right);
    AstBinaryOp(Token token, Operation op, AstExpression *left, AstExpression *right);
    Operation getOperation() const;
    AstExpression* getLeft() const;
    AstExpression* getRight() const;
    ostream& print(ostream& out) const;
};

class AstOperand : public AstExpression{
  public:
    AstOperand(Token token);
};

class AstIdent : public AstOperand{
  private:
    string _name;
  public:
    AstIdent(Token token, string name);
    const string getName() const;
    ostream& print(ostream &out) const;
};

class AstConstant : public AstOperand{
  private:
    string _value;
  public:
    AstConstant(Token t, string value);
    void setValue(string value);
    const string getValue() const;
    ostream& print(ostream &out) const;
};

#endif

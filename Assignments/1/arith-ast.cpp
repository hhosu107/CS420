#include<iostream>
#include<cassert>
#include<cstring>
#include"arith-ast.hpp"
using namespace std;

int AstNode::_global_id = 0;

// AstNode
AstNode::AstNode(Token token): _token(token) {
  _id = _global_id++;
}

int AstNode::getID() const{
  return _id;
}

Token AstNode::getToken() const{
  return _token;
}

ostream& operator<<(ostream &out, const AstNode &t){
  return t.print(out);
}

ostream& operator<<(ostream &out, const AstNode *t){
  return t->print(out);
}

// AstExpression
AstExpression::AstExpression(Token token) : AstNode(token) {

}

// AstBinaryOp
AstBinaryOp::AstBinaryOp(Token token, Operation op, AstExpression *left, AstExpression *right): AstExpression(token), _op(op), _left(left), _right(right) {
  assert((op == operationAdd) || (op == operationSub) ||
      (op == operationMul) || (op == operationDiv));
  assert(right != NULL);
}

Operation AstBinaryOp::getOperation() const{
  return _op;
}

AstExpression* AstBinaryOp::getLeft() const{
  return _left;
}

AstExpression* AstBinaryOp::getRight() const{
  return _right;
}

ostream& AstBinaryOp::print(ostream& out) const{
  // preorder
  out << OperationName[_op];
  _left->print(out);
  _right->print(out);
  return out;
}

// AstOperand
AstOperand::AstOperand(Token token) : AstExpression(token) {
}

// AstIdent
AstIdent::AstIdent(Token token, string name) : AstOperand(token), _name(name) {

}

const string AstIdent::getName() const{
  return _name;
}

ostream& AstIdent::print(ostream &out) const{
  out << getName();
  return out;
}

// AstConstand
AstConstant::AstConstant(Token token, string value) : AstOperand(token), _value(value){

}

void AstConstant::setValue(string value){
  _value = value;
}

const string AstConstant::getValue() const{
  return _value;
}

ostream& AstConstant::print(ostream &out) const{
  out << getValue();
  return out;
}
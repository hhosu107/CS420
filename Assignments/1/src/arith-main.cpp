// From scanner, if some tokens have remained as tUndefined or tError, then stop scanning and just print the "incorrect syntax"

#include<cstdlib>
#include<iostream>
#include<fstream>
#include<cassert>
#include"arith-scanner.hpp"
#include"arith-parser.hpp"
using namespace std;

int main(){
  ifstream _in("input.txt");
  ofstream _out("output.txt");
  string _line;
  Scanner *s = NULL;
  Parser *p = NULL;
  stringstream inStream;
  while(getline(_in, _line)){
    s = new Scanner(_line);
    p = new Parser(s);

    AstNode *goal = p->parse(); // parse
    if(p->hasError()){
      _out << "incorrect syntax" << endl;
    }
    else{ // if there is no error, then goal should not be empty unless the input itself is empty
      assert(goal != NULL);
      goal->print(_out);
      _out << endl;
    }
    delete s;
    delete p;
  }
  _in.close();
  _out.close();
  return EXIT_SUCCESS;
}

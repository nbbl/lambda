#include<cctype>
#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include"ast.hpp"

using namespace std;

enum Token_value{

  LAMBDA='%', BIND='.', VAR,
  LP='(',     RP=')',   SEP=';',
  END
};

Token_value curr_tok = SEP;
string curr_var[1];

void error(const string& msg){
  
  cerr << "error: " << msg;
  exit(1);
}

Token_value get_token(){
  
  char ch;
  
  do{
    if(!cin.get(ch)) return curr_tok=END;
  }while(ch != '\n' && isspace(ch));
  
  switch(ch){
   
  case ';':
  case '\n':
    return curr_tok=SEP;

  case '%':
    return curr_tok=LAMBDA;

  case '.':
    return curr_tok=BIND;

  case '(':
    return curr_tok=LP;

  case ')':
    return curr_tok=RP;

  default:{
    if(isalpha(ch)){
      curr_var[0] = ch;
      while(cin.get(ch) && isalnum(ch)){
        curr_var[0].push_back(ch);
      }
      cin.putback(ch);
      return curr_tok=VAR;
    }
    
    error("bad token");
    return curr_tok=SEP;
  }
  }
}

DBTerm* parse_expr(bool get, list<string> vars);

DBTerm* parse_term(bool get, list<string> vars){

  DBTerm* root = new DBTerm;

  if(get) get_token();
  switch(curr_tok){

  case LAMBDA:{
    if(get_token() != VAR) error("variable expected");
    if(get_token() != BIND) error("binder expected");
    root->kind = ABS;
    vars.push_back(curr_var[0]);
    root->lterm = parse_expr(true, vars);
    return root;
  }

  case VAR:{
    root->kind = INDEX;
    //if(vars.empty()) return root;
    list<string>::iterator i = find_end(vars.begin(), vars.end(), curr_var, curr_var+1);
    int j = vars.size() - distance(vars.begin(), i);
    if(j <= 0) error("variable not in scope: " + curr_var[0]); 
    else root->index = j;
    get_token();
    return root;
  }

  case LP:{
    root = parse_expr(true, vars);
    if(curr_tok != RP) error("')' expected"); 
    get_token();
    return root;
  }

  default:{
    error("illegal token");
    return root;
  }
  }
}

DBTerm* parse_expr(bool get, list<string> vars){

  DBTerm* root = new DBTerm;
  root->lterm = parse_term(get, vars);
  
  switch(curr_tok){
    
  case SEP:
  case END:
  case RP:
    return root->lterm;

  default:
    root->rterm = parse_expr(false, vars);
    root->kind = APP;
    return root;
    
  }
}

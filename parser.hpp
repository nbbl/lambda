/*
  parser.hpp
  recursive descent parser,
  uses DBExpr from ast.hpp as datastructure for the expressions.
*/
#include<cctype>
#include<iostream>
#include<string>
#include<list>
#include<vector>
#include<algorithm>
#include<cstdlib>
#include"ast.hpp"

using namespace std;

// Token_value contains all lexical tokens of the language
enum Token_value{

  LAMBDA='%', BIND='.', VAR,
  LP='(',     RP=')',   SEP='\n',
  END
};

/*
 curr_tok is the last read token,
 initialized to SEP to start in a clean state
*/
Token_value curr_tok = SEP;

/*
  curr_var[] holds the last read variable,
  array because of the arg type of find_end
*/
string curr_var[1];

void error(const string& msg){
  
  cerr << "error: " << msg;
  exit(1);
}

/* 
   get_token() is the basic lexer,
   removes whitespace, sets/returns 
   the next curr_tok and curr_var if
   necessary
*/
Token_value get_token(){
  
  char ch;
  
  do{
    if(!cin.get(ch)) return curr_tok=END;
  }while(ch != '\n' && isspace(ch));
  
  switch(ch){
   
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

DBExpr* parse_expr(bool get, list<string> vars);

/*
  parse_term() parses abstractions and variables,
  get signals whether the next token must be 
  gotten first
*/
DBExpr* parse_term(bool get, list<string> vars){

  DBExpr* root = new DBExpr;

  if(get) get_token();
  switch(curr_tok){

  case LAMBDA:{
    if(get_token() != VAR) error("variable expected");
    if(get_token() != BIND) error("binder expected");
    root->kind = ABS;
    vars.push_back(curr_var[0]);
    root->lexpr = parse_expr(true, vars);
    return root;
  }

  case VAR:{
    root->kind = INDEX;
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

/*
  parse_expr() parses applications
*/
DBExpr* parse_expr(bool get, list<string> vars){

  DBExpr* root = new DBExpr;
  root->lexpr = parse_term(get, vars);
  
  switch(curr_tok){
    
  case SEP:
  case END:
  case RP:
    return root->lexpr;

  default:
    root->rexpr = parse_expr(false, vars);
    root->kind = APP;
    return root;
    
  }
}

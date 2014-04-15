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
string string_value;

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
      string_value = ch;
      while(cin.get(ch) && isalnum(ch)){
        string_value.push_back(ch);
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
  cout << "after decl of root in parse_term \n";

  if(get) get_token();
  switch(curr_tok){

  case LAMBDA:{
    cout << "after case LAMBDA in parse term \n";
    if(get_token() != VAR) error("variable expected");
    if(get_token() != BIND) error("binder expected");
    root->kind = ABS;
    vars.push_back(string_value);
    root->lterm = parse_expr(true, vars);
    return root;
  }

  case VAR:{
    root->kind = INDEX;
    if(vars.empty()) return root;
    list<string> s = {string_value};
    list<string>::iterator i = find_end(vars.begin(), vars.end(), s.begin(), s.end());
    root->index = distance(vars.begin(), i) + 1;
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
  cout << "after decl of root in parse_expr \n";
  root->lterm = parse_term(get, vars);
  
  switch(curr_tok){
    
  case SEP:
  case END:
    return root->lterm;

    //here are some cases missing: VAR, LP ..?

  default:
    string s;
    switch(curr_tok){
    case END: s = "END"; break;
    case LAMBDA: s = "LAMBDA"; break;
    case SEP: s = "SEP"; break;
    case LP: s = "LP"; break;
    case RP: s = "RP"; break;
    case VAR: s = "VAR: " + string_value; break;
    default: s = "unknown"; break;
    }
    cout << "after default in parse_expr, curr_tok == " << s << '\n';
    root->rterm = parse_expr(false, vars);
    root->kind = APP;
    return root;
    
  }
}

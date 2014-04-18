#include<string>

using namespace std;

/*
  DBExpr is the data structure for 
  the ast using de Bruijn indices

  INDEX:
    index is the de B. index of the variable
    lexpr, rexpr not used
  APP:
    lexpr is the left, rexpr is right part
    of the application, index not used
  ABS: 
    lexpr ist the expression under the binder,
    rterm and index not used
*/
enum Kind{ INDEX, APP, ABS };

struct DBExpr{
  
  Kind kind;
  int index;
  DBExpr* lexpr;
  DBExpr* rexpr;  

}; 

string flatten_expr(DBExpr* expr){
    
  switch(expr->kind){

  case INDEX:
    return to_string(expr->index);

  case ABS:
    return "% " + flatten_expr(expr->lexpr);
 
  case APP:
    string lstr, rstr;
    if(expr->lexpr->kind == INDEX) lstr = flatten_expr(expr->lexpr) + " ";
    else lstr = "( " + flatten_expr(expr->lexpr) + ") ";
    if(expr->rexpr->kind == INDEX) rstr = flatten_expr(expr->rexpr) + " ";
    else rstr = "( " + flatten_expr(expr->rexpr) + ") ";
    return lstr + rstr;
   
  }
}

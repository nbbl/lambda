#include<string>

using namespace std;

enum Kind{ INDEX, APP, ABS };

struct DBTerm{
  
  Kind kind;
  int index;
  DBTerm* lterm;
  DBTerm* rterm;  

}; 

string flatten_term(DBTerm* term){
    
  switch(term->kind){

  case INDEX:
    return to_string(term->index) + " ";

  case ABS:
    return "% " + flatten_term(term->lterm);
 
  case APP:
    return " (" + flatten_term(term->lterm) + ") (" + flatten_term(term->rterm) + ") ";
   
  }
}

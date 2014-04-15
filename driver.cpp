#include"parser.hpp"

using namespace std;

int main(){

  while(cin){
    cout << flatten_term(parse_expr(true, {})) << "\n";
    
  }
}

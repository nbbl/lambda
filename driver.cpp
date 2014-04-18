#include"parser.hpp"

using namespace std;

int main(){

  while(cin){
    cout << flatten_expr(parse_expr(true, {})) << "\n";
    
  }
}

// #include "analyzer.h"
#include "coords.h"
#include "lexer.h"
#include "parser.h"
#include <sstream>
#include <string>



int main() {

  std::istringstream iss(
                          "CartProd {\n"
                          "\t(t.X e.X) (e.Y) = <CartProd-Bind t.X e.Y> <CartProd (e.X) (e.Y)>;\n"
                          "(e.Y) = ;\n"
                          "}");
  //
  std::string* attr = new std::string();
  Coords* coords = new Coords(0, 0, 0, 0);
  Lexer lexer = Lexer(&iss);
  auto p = yy::Parser(lexer, coords, attr);
  p.parse();
  return 0;
}

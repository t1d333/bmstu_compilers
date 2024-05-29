#include "analyzer.h"
#include "ast.h"
#include "coords.h"
#include "domain.h"
#include "lexer.h"
#include "token.h"
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <stack>
#include <string>



int main() {

  std::istringstream iss(
                          " CartProd {\n"
                          "\te.Y = <CartProd-Bind t.X e.Y>;\n"
                          "}");

  
  std::string* attr = new std::string();
  Coords* coords = new Coords(0, 0, 0, 0);
  // std::unique_ptr<Lexer> lexer = std::make_unique<Lexer>(&std::iss);
  
  Analyzer *analyzer = new Analyzer();
  auto prog =  analyzer->analyze();
  prog->print();
  
  
  // std::stack<std::pair<std::shared_ptr<Node>, int>> s;
  // s.push(std::make_pair(root, 0));
  //
  // int i = 0;
  //
  // while (!s.empty()) {
  //   std::shared_ptr<Node> top = s.top().first;
  //   int level = s.top().second;
  //   s.pop();
  //
  //   for (int j = 0; j < level; j++) {
  //     std::cout << "\t";
  //   }
  //
  //   top->print();
  //   std::cout << std::endl;
  //
  //   std::vector<std::shared_ptr<Node>> childs = top->get_childs();
  //   int j = childs.size() - 1;
  //
  //   for (; j >= 0; j--) {
  //     s.push(std::make_pair(childs[j], level + 1));
  //   }
  // }
  
  return 0;
}

#include "analyzer.h"
#include "coords.h"
#include "domain.h"
#include "inner.h"
#include "leaf.h"
#include "lexer.h"
#include "table.h"
#include "token.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <ostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <utility>

Analyzer::Analyzer()
    : stack(std::vector<std::pair<std::shared_ptr<Node>, Symbol>>()),
      lexer(std::make_unique<Lexer>()) {}

Analyzer::Analyzer(std::istream *in)
    : stack(std::vector<std::pair<std::shared_ptr<Node>, Symbol>>()),
      lexer(std::make_unique<Lexer>(in)) {}

std::shared_ptr<Node> Analyzer::analyze() {
  std::shared_ptr<Node> parent = std::make_shared<Inner>();
  std::shared_ptr<Node> result = parent;

  this->stack.push_back(std::make_pair(parent, Symbol::END));
  this->stack.push_back(std::make_pair(parent, Symbol::PROG));

  Coords coords = Coords(0, 0, 0, 0);
  std::string *attr = new std::string;

  DomainTag tag = this->lexer->next_token(&coords, attr);
  Symbol s = this->stack.rbegin()->second;

  do {
    parent = this->stack.rbegin()->first;
    s = this->stack.rbegin()->second;

    if (s >= Symbol::END) {
      if (s == Symbol::END + static_cast<Symbol>(tag)) {
        this->stack.pop_back();
        Token t(tag, coords, *attr);
        std::shared_ptr<Node> leaf = std::make_shared<Leaf>(t);

        parent->add_child(leaf);
        tag = this->lexer->next_token(&coords, attr);
      } else {

        std::cout << "Error at position " << coords << std::endl;
        // DomainTag expected = static_cast<DomainTag>(s - Symbol::END);
        // std::cout << "Expected: " << tag_to_string(expected)
        // << "\nReceived: " << tag_to_string(tag) << std::endl;
        return nullptr;
      }
    } else if (TABLE[s][static_cast<Symbol>(tag)][0] != Symbol::ERROR) {

      this->stack.pop_back();

      std::shared_ptr<Inner> inner = std::make_shared<Inner>(s);

      parent->add_child(inner);

      std::vector<Symbol> rule_right = std::vector<Symbol>();
      for (int i = 0; i < MAX_CELL_SIZE; ++i) {

        Symbol curr = TABLE[s][static_cast<Symbol>(tag)][i];

        if (curr == Symbol::EMPTY) {

          break;
        }
        rule_right.push_back(curr);
      }
      std::reverse(rule_right.begin(), rule_right.end());

      for (int i = 0; i < rule_right.size(); ++i) {
        this->stack.push_back(std::make_pair(inner, rule_right[i]));
      }

    } else {
      std::cout << "Error at position " << coords << std::endl;
      // DomainTag expected = static_cast<DomainTag>(s - Symbol::END);
      // std::cout << "Expected: " << tag_to_string(expected)
      // << "\nReceived: " << tag_to_string(tag) << std::endl;

      return nullptr;
    }

  } while (s != Symbol::END);

  return parent;
}

int main() {

  std::istringstream iss("tokens <plus sign>, <star>, <n> .");

  Analyzer *anal = new Analyzer(&iss);

  std::shared_ptr<Node> root = anal->analyze();

  if (root == nullptr) {
    return 0;
  }

  std::stack<std::pair<std::shared_ptr<Node>, int>> s;
  s.push(std::make_pair(root, 0));

  int i = 0;

  while (!s.empty()) {
    std::shared_ptr<Node> top = s.top().first;
    int level = s.top().second;
    s.pop();

    for (int j = 0; j < level; j++) {
      std::cout << "\t";
    }

    top->print();
    std::cout << std::endl;

    std::vector<std::shared_ptr<Node>> childs = top->get_childs();
    int j = childs.size() - 1;

    for (; j >= 0; j--) {
      s.push(std::make_pair(childs[j], level + 1));
    }
  }
  return 0;
}

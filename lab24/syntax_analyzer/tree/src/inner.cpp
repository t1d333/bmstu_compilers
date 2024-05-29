#include "inner.h"
#include "node.h"
#include "symbol.h"
#include <iostream>

Inner::Inner(Symbol s) : s(s), children(0), Node(){};
Inner::Inner() : children(0), Node() {}

void Inner::print() {
  std::cout << "Inner node{ " + symbol_to_string(this->s) << " }";
}

std::vector<std::shared_ptr<Node>> const &Inner::get_childs() {
  return this->children;
};

void Inner::add_child(std::shared_ptr<Node> node) {
  this->children.push_back(node);
};

Inner::~Inner() {
  
}

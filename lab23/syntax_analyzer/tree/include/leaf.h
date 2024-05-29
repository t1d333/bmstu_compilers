#pragma once

#include "node.h"
#include "token.h"

class Leaf : public Node {
public:
  Leaf(Token);
  Leaf(Leaf &&) = default;
  Leaf(const Leaf &) = default;
  Leaf &operator=(Leaf &&) = default;
  Leaf &operator=(const Leaf &) = default;
  std::vector<std::shared_ptr<Node>> const &get_childs() {
    return this->children;
  };

  friend std::ostream &operator<<(std::ostream &os, const Leaf &leaf) {
    os << leaf.t;
    return os;
  }

  virtual void print() { std::cout << "LEAF: " << this->t; };

  void add_child(std::shared_ptr<Node> node){};
  ~Leaf() {};

private:
  Token t;
  std::vector<std::shared_ptr<Node>> children;
};

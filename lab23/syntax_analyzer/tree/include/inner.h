#pragma once

#include "node.h"
#include "symbol.h"

class Inner : public Node {
public:
  Inner();
  Inner(Symbol s);
  Inner(Inner &&) = default;
  Inner(const Inner &) = default;
  Inner &operator=(Inner &&) = default;
  Inner &operator=(const Inner &) = default;
  void print();
  std::vector<std::shared_ptr<Node>> const &get_childs();
  void add_child(std::shared_ptr<Node> node);

  ~Inner();

private:
  Symbol s;
  std::vector<std::shared_ptr<Node>> children;
};

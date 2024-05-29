#pragma once

#include <memory>
#include <string>
#include <vector>

class Node {
public:
  Node() = default;
  Node(Node &&) = default;
  Node(const Node &) = default;
  Node &operator=(Node &&) = default;
  Node &operator=(const Node &) = default;
  virtual void print() = 0;
  virtual void add_child(std::shared_ptr<Node> node) = 0;
  virtual std::vector<std::shared_ptr<Node>> const &get_childs() = 0;
  virtual ~Node(){};

private:
};

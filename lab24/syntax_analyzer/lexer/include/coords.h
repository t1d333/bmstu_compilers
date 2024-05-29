#pragma once

#include <iostream>
#include <utility>

class Coords {
public:
  Coords(int l1, int p1, int l2, int p2);
  Coords(std::pair<int, int> start, std::pair<int, int> end);
  Coords(Coords &&) = default;
  Coords(const Coords &) = default;
  Coords &operator=(Coords &&) = default;
  Coords &operator=(const Coords &) = default;
  std::pair<int, int> const &get_start() { return this->start; };
  std::pair<int, int> const &get_end() { return this->end; };
  friend std::ostream &operator<<(std::ostream &os, const Coords &c) {
    os << "(" << c.start.first << ", " << c.start.second << ")"
       << "-"
       << "(" << c.end.first << ", " << c.end.second << ")";
    return os;
  };
  ;
  ~Coords(){};

private:
  std::pair<int, int> start;
  std::pair<int, int> end;
};

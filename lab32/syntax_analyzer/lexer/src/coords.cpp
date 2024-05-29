#include "coords.h"

Coords::Coords(int l1, int p1, int l2, int p2) : start(l1, p1), end(l2, p2){};
Coords::Coords(std::pair<int, int> start, std::pair<int, int> end)
    : start(start), end(end){};

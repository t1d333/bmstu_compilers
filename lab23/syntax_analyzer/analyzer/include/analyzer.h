#include "lexer.h"
#include "node.h"
#include "symbol.h"
#include "vector"
#include <istream>
#include <memory>
#include <utility>

class Analyzer {
public:
  Analyzer(std::istream *in);
  Analyzer();
  ~Analyzer();

  std::shared_ptr<Node> analyze();

private:
  std::unique_ptr<Lexer> lexer;
  std::vector<std::pair<std::shared_ptr<Node>, Symbol>> stack;
};

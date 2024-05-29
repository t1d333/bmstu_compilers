#include "coords.h"
#include "domain.h"
#include "lexer.h"
#include "node.h"
#include <istream>
#include <memory>
#include <string>
#include "token.h"

class Analyzer {
public:
  Analyzer(std::istream *in);
  Analyzer();
  ~Analyzer();

  
  std::shared_ptr<Node> analyze();
  std::shared_ptr<Node> ParseStart();
  std::shared_ptr<Node> ParseFunc();
  std::shared_ptr<Node> ParseFuncBody();
  std::shared_ptr<Node> ParseBodyPart();
  std::shared_ptr<Node> ParseBodyPartRhs();
  std::shared_ptr<Node> ParseBodyPartEnd();
  std::shared_ptr<Node> ParseResult();
  std::shared_ptr<Node> ParseResultPart();
  std::shared_ptr<Node> ParsePattern();
  std::shared_ptr<Node> ParsePatternPart();

private:
  void next_token() {
    DomainTag tag = this->lexer->next_token(this->coords, this->attr);
    this->curr = Token(tag, *this->coords, *this->attr);
  };
  
  void print_err(DomainTag expected) {
    std::cout << "ERROR!\nCurrent Token: " << this->curr << "\nExpected: " << Token(expected ,*this->coords, *this->attr)<< std::endl;
  }
  
  Coords* coords;
  std::string* attr;
  std::unique_ptr<Lexer> lexer;
  Token curr;
};

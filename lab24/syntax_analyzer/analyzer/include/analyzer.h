#pragma once

#include "coords.h"
#include "domain.h"
#include "lexer.h"
#include <istream>
#include <memory>
#include <string>
#include "token.h"
#include "ast.h"

class Analyzer {
public:
  Analyzer(std::istream *in);
  Analyzer();
  ~Analyzer();

  
  std::unique_ptr<ast::Programm> analyze();
  std::unique_ptr<ast::Programm> ParseStart();
  std::unique_ptr<ast::Func> ParseFunc();
  std::unique_ptr<ast::FuncBody> ParseFuncBody();
  std::unique_ptr<ast::Sentence> ParseBodyPart();
  std::pair<std::unique_ptr<ast::SentenceRhs>, std::vector<std::unique_ptr<ast::Condition>>> ParseBodyPartRhs();
  std::unique_ptr<ast::SentenceRhs> ParseBodyPartEnd();
  std::unique_ptr<ast::Result> ParseResult();
  std::unique_ptr<ast::Pattern> ParsePattern();

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

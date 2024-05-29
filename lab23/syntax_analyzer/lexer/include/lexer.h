#pragma once

#include "coords.h"
#include "domain.h"
#include <utility>

#ifndef FLEX_LEXER
#define FLEX_LEXER
#include <FlexLexer.h>

#endif // FLEX_LEXER

#ifndef YY_USER_ACTION

#endif // YY_USER_ACTION

#undef YY_DECL
#define YY_DECL DomainTag Lexer::next_token(Coords *coords, std::string *attr)

#define YY_USER_ACTION                                                         \
  {                                                                            \
    int i;                                                                     \
    std::string text = YYText();                                               \
    std::pair<int, int> start = std::make_pair(this->line, this->pos);         \
    for (i = 0; i < YYLeng(); ++i) {                                           \
      if (text[i] == '\n') {                                                   \
        ++this->line;                                                          \
        this->pos = 1;                                                         \
      } else {                                                                 \
        ++this->pos;                                                           \
      }                                                                        \
      ++this->idx;                                                             \
    }                                                                          \
    std::pair<int, int> end = std::make_pair(this->line, this->pos);           \
    *coords = Coords(start, end);                                              \
  }

class Lexer : public yyFlexLexer {
public:
  Lexer() : yyFlexLexer() {}
  Lexer(std::istream *in) : in_stream(in), yyFlexLexer(in) {}

  Lexer(Lexer &&) = delete;
  Lexer(const Lexer &) = delete;
  Lexer &operator=(Lexer &&) = delete;
  Lexer &operator=(const Lexer &) = delete;
  DomainTag next_token(Coords *coords, std::string *attr);
  ~Lexer() {}

private:
  std::istream *in_stream;
  int line = 1;
  int pos = 1;
  int idx = 0;
};

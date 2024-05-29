#pragma once
#include "coords.h"
#include "domain.h"
#include <iostream>

class Token {
public:
  Token(DomainTag kind, Coords c, std::string attr = "")
      : kind(kind), coords(c), attr(attr){};
  Token(Token &&) = default;
  Token(const Token &) = default;
  Token &operator=(Token &&) = default;
  Token &operator=(const Token &) = default;
  friend std::ostream &operator<<(std::ostream &os, const Token &t) {
    std::string tag = "";
    switch (t.kind) {
    case DomainTag::_EOF:
      tag = "eof";
      break;
    case DomainTag::COMMA:
      tag = "comma";
      break;
    case DomainTag::O_BRACKET:
      tag = "o_bracket";
      break;
    case DomainTag::C_BRACKET:
      tag = "c_bracket";
      break;
    case DomainTag::IS:
      tag = "is";
      break;
    case DomainTag::START_KW:
      tag = "start";
      break;
    case DomainTag::DOT:
      tag = "dot";
      break;
    case DomainTag::TOKENS_KW:
      tag = "tokens";
      break;
    case DomainTag::TOKEN_NAME:
      tag = "token_name";
      break;
    case DomainTag::NONTERMINAL_NAME:
      tag = "nonterminal_name";
      break;
    case DomainTag::EXCLAMATION_POINT:
      tag = "EXCLAMATION_POINT";
      break;
    case DomainTag::MINUS:
      tag = "minux";
      break;
    case DomainTag::COMMENT_BODY:
      tag = "comment_body";
      break;
    }
    os << tag << " ";
    os << t.coords << std::endl;
    return os;
  };
  ;
  ~Token() {}

private:
  DomainTag kind;
  Coords coords;
  std::string attr;
};

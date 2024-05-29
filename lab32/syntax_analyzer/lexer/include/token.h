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
      os << "EOF";
      break;
    case DomainTag::INTEGER:
      os << "INTEGER";
      os << "( " << t.attr << " ) ";
      break;
    case DomainTag::IDENT:
      os << "IDENT";
      os << "( " << t.attr << " ) ";
      break;
    case DomainTag::STRING:
      os << "STRING ";
      break;
    case DomainTag::COMMA:
      os << "COMMA ";
      break;
    case DomainTag::SEMICOLON:
      os << "SEMICOLON";
      break;
    case DomainTag::COLON:
      os << "COLON ";
      break;
    case DomainTag::DOT:
      os << "DOT ";
      break;
    case DomainTag::O_ANGLE_BRACKET:
      os << "O_ANGLE_BRACKET ";
      break;
    case DomainTag::C_ANGLE_BRACKET:
      os << "C_ANGLE_BRACKET ";
      break;
    case DomainTag::O_CURLY_BRACKET:
      os << "O_CURLY_BRACKET ";
      break;
    case DomainTag::C_CURLY_BRACKET:
      os << "C_CURLY_BRACKET ";
      break;
    case DomainTag::O_BRACKET:
      os << "O_BRACKET ";
      break;
    case DomainTag::C_BRACKET:
      os << "C_BRACKET ";
      break;
    case DomainTag::END:
      os << "END ";
      break;
    case DomainTag::EQUAL:
      os << "EQUAL ";
      break;
    case DomainTag::VAR:
      os << "VAR";
      os << "( " << t.attr << " ) ";
      break;
    }
    os << tag << " ";
    os << t.coords; 
    return os;
  };
  ;
  ~Token() {}

  DomainTag get_kind() {return this->kind;};
private:
  DomainTag kind;
  Coords coords;
  std::string attr;
};

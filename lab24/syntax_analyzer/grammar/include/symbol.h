#include <string>
#pragma once


enum Symbol {
  // Nonterminals
  START,
  FUNC,
  FUNC_BODY,
  BODY_PART_RHS,
  BODY_PART_END,
  RESULT,
  RESULT_PART,
  PATTERN,
  PATTER_PART,

  // Terminals
  END,
  INTEGER,
  VAR,
  IDENT,
  STRING,
  COMMA,
  SEMICOLON,
  COLON,
  DOT,
  O_ANGLE_BRACKET,
  C_ANGLE_BRACKET,
  O_CURLY_BRACKET,
  C_CURLY_BRACKET,
  O_BRACKET,
  C_BRACKET,
  EQUAL,
};


std::string symbol_to_string(Symbol& s);

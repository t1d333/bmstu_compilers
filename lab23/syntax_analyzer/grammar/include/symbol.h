#include <string>
#pragma once


enum Symbol {
  // Nonterminals
  PROG,
  STMNT,
  TOKENS,
  STMNT_INNER,
  RULE_INNER,
  BODY,
  BODIES,
  BODY_INNER,
  COMMENT_INNER,

  // Terminals

  END,
  TOKENS_KW,
  TOKEN_NAME,
  O_BRACKET,
  C_BRACKET,
  COMMA,
  START_KW,
  NONTERMINAL_NAME,
  IS,
  DOT,
  EXCLAMATION_POINT,
  MINUS,
  COMMENT_BODY,

  EMPTY,
  ERROR,
};


std::string symbol_to_string(Symbol& s);

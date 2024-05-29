#include "symbol.h"

std::string symbol_to_string(Symbol &s) {
  switch (s) {
  case PROG:
    return "PROG";
  case STMNT:
    return "STMNT";
  case TOKENS:
    return "TOKENS";
  case STMNT_INNER:
    return "STMNT_INNER";
  case RULE_INNER:
    return "RULE_INNER";
  case BODY:
    return "BODY";
  case BODIES:
    return "BODIES";
  case BODY_INNER:
    return "BODY_INNER";
  case COMMENT_INNER:
    return "COMMENT_INNER";
  case END:
    return "END";
  case TOKENS_KW:
    return "TOKENS_KW";
  case TOKEN_NAME:
    return "TOKENS_NAME";
  case O_BRACKET:
    return "OPEN_BRACKET";
  case C_BRACKET:
    return "CLOSE_BRACKET";
  case COMMA:
    return "COMMA";
  case START_KW:
    return "START_KW";
  case NONTERMINAL_NAME:
    return "NONTERMINAL_NAME";
  case IS:
    return "IS";
  case DOT:
    return "DOT";
  case EXCLAMATION_POINT:
    return "EXCLAMATION_POINT";
  case MINUS:
    return "MINUS";
  case COMMENT_BODY:
    return "COMMENT_BODY";
  case EMPTY:
    return "EMPTY";
  case ERROR:
    return "ERROR";
  default:
    return "undefined symbol";
  }
}

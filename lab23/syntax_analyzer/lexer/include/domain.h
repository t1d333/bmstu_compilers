#pragma once

#include <string>

// PROG -> STMNT "dot" PROG .
// PROG -> .
// STMNT -> "tokens" "left" "token_name" "right" TOKENS .
// TOKENS ->  "comma" "left" "token_name" "right" TOKENS .
// TOKENS -> .
// STMNT -> "start" left" "nonterminal_name" "right" .
// STMNT -> "left" STMNT_INNER .
// STMNT_INNER -> COMMENT_INNER .
// STMNT_INNER -> RULE_INNER .
// RULE_INNER -> "nonterminal_name" "right" "is" BODY .
// BODY ->  BODY_TMP .
// BODY_TMP -> "left" BODY_INNER "right" BODY_TMP .
// BODY_INNER -> "nonterminal_name" .
// BODY_INNER -> "token_name" .
// BODY_TMP -> .
// COMMENT_INNER ->  "exclamation_point" "minus" "minus" "comment_inner" "minus"
// "minus" "right" .

enum class DomainTag {
  _EOF,
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
};



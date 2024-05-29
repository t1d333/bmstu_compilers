#pragma once

// integer {digit}*
// ident [A-Za-z][A-Za-z0-9_-]+
// string '[^\n']*'|\"[^\n\"]*\"
// comma ,
// semicolon ;
// colon :
// dot \.
// o_angle_bracket <
// c_angle_bracket >
// o_curly_bracket {
// c_curly_bracket }
// o_bracket (
// c_bracket )
// end \$

enum DomainTag {
  _EOF,
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
  END,
};



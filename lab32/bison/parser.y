%skeleton "lalr1.cc" // -*- C++ -*-
%require "3.8.1"
%header
%define api.parser.class { Parser }

%define api.token.raw

%code requires {
  class Lexer;
  class Corrds;
    
}

%code top {
  #include "coords.h"
  #include "lexer.h"

  static yy::Parser::symbol_type yylex(Lexer &lexer, Coords *coords, std::string *attr) {
        return lexer.next_token(coords, attr);
    }
    
}

%parse-param  { Lexer &lexer      }
%parse-param  { Coords *coords    }
%parse-param  { std::string *attr }
%lex-param    { Lexer &lexer      }
%lex-param    { Coords *coords    }
%lex-param    { std::string *attr }
%define api.value.type variant
%define parse.assert
%define api.token.constructor


%token END 0 "end of file"
%token  ASSIGN            "="
        LEFT_PAREN        "("
        RIGHT_PAREN       ")"
        LEFT_ANGLE_PAREN  "<"
        RIGHT_ANGLE_PAREN ">"
        LEFT_CURLY_PAREN  "{"
        RIGHT_CURLY_PAREN "}"
        COMMA         ","
        COLON         ":"
        SEMICOLON     ";"
        DOT           "."
%token  <std::string> STRING  "string"
        <std::string> IDENT   "ident"
        <std::string> VAR     "var"
        <long>        INTEGER "integer"

%%


start:
     func start {  }
     | END
     ;

func:
    IDENT LEFT_CURLY_PAREN func_body RIGHT_CURLY_PAREN {std::cout << $1 << "{\n" << $2 << "\n}"}
    ;

func_body:
         | sentence func_body {std::cout << "\t" << $1 << "\n" << $2 }
         ;

sentence:
        lhs ASSIGN result SEMICOLON
        | lhs COMMA result COLON LEFT_CURLY_PAREN func_body RIGHT_CURLY_PAREN SEMICOLON "" 
        ;

lhs:
   pattern 
   | lhs COMMA condition
   ;

condition:
         result COLON pattern
         ;

result:
      |result_part result
      ;

result_part:
           IDENT
           | STRING
           | INTEGER 
           | VAR { std::cout << "result: "<< $1 << std::endl;} 
           | LEFT_PAREN result RIGHT_PAREN 
           | LEFT_ANGLE_PAREN IDENT result RIGHT_ANGLE_PAREN
           ;

pattern:
       | pattern_part pattern
       ;

pattern_part:
           IDENT
           | STRING
           | INTEGER
           | VAR { std::cout << "pattern: "<< $1 << std::endl;} 
           | LEFT_PAREN pattern RIGHT_PAREN 
           | LEFT_ANGLE_PAREN IDENT pattern RIGHT_ANGLE_PAREN
           ;


%%

void yy::Parser::error(const std::string &err_message )
{
   std::cerr << "Error: " << err_message << "\n";
}

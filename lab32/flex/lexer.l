%{
#include "domain.h"
#define FLEX_LEXER
#include "lexer.h"
#include "parser.h"
%}

%option yyclass="Lexer"
%option c++
%option noyywrap
%option noinput nounput


%x NAME

digit [0-9]
integer {digit}*
var (s|e|t)\.(([A-Za-z][A-Za-z0-9_-]*)|([0-9]+))
ident [A-Za-z][A-Za-z0-9_-]*
string '[^\n']*'|\"[^\n\"]*\"
comma ,
semicolon ;
colon :
dot \.
o_angle_bracket \<
c_angle_bracket \>
o_curly_bracket \{
c_curly_bracket \}
o_bracket \(
c_bracket \)
equal =
end \$
commment_begin /\*
commment_end \*/


%%

[\n\t ]+


{integer}                 {
                            return yy::Parser::make_INTEGER(1);
                          }
{ident}                   {
                            return yy::Parser::make_IDENT(YYText());
                          }
{var}                     {
                            return yy::Parser::make_VAR(YYText());
                          }
{string}                  {
                            return yy::Parser::make_STRING(YYText());
                          }
{comma}                   {
                            return yy::Parser::make_COMMA();
                          }
{semicolon}               {
                            return yy::Parser::make_SEMICOLON();
                          }
{colon}                   {
                            return yy::Parser::make_COLON();
                          }
{dot}                     {
                            return yy::Parser::make_DOT();
                          }
{o_angle_bracket}         {
                            return yy::Parser::make_LEFT_ANGLE_PAREN();
                          }
{c_angle_bracket}         {
                            return yy::Parser::make_RIGHT_ANGLE_PAREN();
                          }
{o_curly_bracket}         {
                            return yy::Parser::make_LEFT_CURLY_PAREN();
                          }
{c_curly_bracket}         {
                            return yy::Parser::make_RIGHT_CURLY_PAREN();
                          }
{c_bracket}               {
                            return yy::Parser::make_RIGHT_PAREN();
                          }
{o_bracket}               {
                            return yy::Parser::make_LEFT_PAREN();
                          }
{equal}                   {
                            return yy::Parser::make_ASSIGN();
                          }
.                         {
                          }
<<EOF>>                   {
                            return yy::Parser::make_END();
                          }
%%


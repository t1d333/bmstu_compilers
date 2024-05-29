
%{
#include "domain.h"
#define FLEX_LEXER
#include "lexer.h"
%}

%option yyclass="Lexer"
%option c++
%option noyywrap

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


%%

[\n\t ]+

{integer}                 {
                            return DomainTag::INTEGER;
                          }
{ident}                   {
                            return DomainTag::IDENT;
                          }

{var}                     {
                            return DomainTag::VAR;
                          }
{string}                  {
                            return DomainTag::STRING;
                          }
{comma}                   {
                            return DomainTag::COMMA;
                          }
{semicolon}               {
                            return DomainTag::SEMICOLON;
                          }
{colon}                   {
                            return DomainTag::COLON;
                          }
{dot}                     {
                            return DomainTag::DOT;
                          }
{o_angle_bracket}         {
                            return DomainTag::O_ANGLE_BRACKET;
                          }
{c_angle_bracket}         {
                            return DomainTag::C_ANGLE_BRACKET;
                          }
{o_curly_bracket}         {
                            return DomainTag::O_CURLY_BRACKET;
                          }
{c_curly_bracket}         {
                            return DomainTag::C_CURLY_BRACKET;
                          }
{c_bracket}               {
                            return DomainTag::C_BRACKET;
                          }
{o_bracket}               {
                            return DomainTag::O_BRACKET;
                          }
{equal}                   {
                            return DomainTag::EQUAL;
                          }
.                         {
                          }
<<EOF>>                   {
                            return DomainTag::_EOF;
                          }
%%


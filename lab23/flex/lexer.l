
%{
#include "domain.h"
#define FLEX_LEXER
#include "lexer.h"
%}

%option yyclass="Lexer"
%option c++
%option noyywrap

%x NAME
%x COMMENT

digit [0-9]
number {digit}*
comma ,
dot \.
o_bracket <
c_bracket >
commet_inner [^\n-]*
minus -
token_name [a-z0-9 ]+
nonterminal_name [A-Z0-9 ]+
comment_inner ([^\n-])*
start start
is "is"
tokens "tokens"
end \$


%%

[\n\t ]+

{comma}                   {
                            return DomainTag::COMMA;
                          }
{dot}                     {
                            return DomainTag::DOT;
                          }
{is}                      {
                            return DomainTag::IS;
                          }
{start}                   {
                            return DomainTag::START_KW;
                          }
{tokens}                  {
                            return DomainTag::TOKENS_KW;
                          }
{o_bracket}               {
                            BEGIN(NAME);
                            return DomainTag::O_BRACKET;
                          }
<NAME>{c_bracket}         {
                            BEGIN(INITIAL);
                            return DomainTag::C_BRACKET;
                          }
<NAME>{nonterminal_name}  {
                            *attr = YYText(); 
                            return DomainTag::NONTERMINAL_NAME;
                          }
<NAME>{token_name}        {
                            *attr = YYText(); 
                            return DomainTag::TOKEN_NAME;
                          }
.                         {
                            std::cout << "undefined symbol " << YYText() << "at pos(" << this->line << ", " << this->pos << ")" << '\n';
                          }
<<EOF>>                   {
                            return DomainTag::_EOF;
                          }
%%


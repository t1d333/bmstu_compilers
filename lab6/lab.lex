%option noyywrap bison-bridge bison-locations


%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define IDENT_ARRAY_SIZE 1024
#define MAX_IDENT_SIZE 64
char* IDENT_ARRAY[IDENT_ARRAY_SIZE];
size_t len = 0;

enum DOMAIN {
  _EOF,
  IDENT,
  WHILE,
  DO,
  END,
  NUMBER,
  COMMENT,
};

int add_ident(char* ident) {
  
  char* tmp = (char*)calloc(MAX_IDENT_SIZE, sizeof(char));
  strcpy(tmp, ident);
  
  if (len > IDENT_ARRAY_SIZE) {
    return -1;
  }
  
  for (int i = 0; i < len; i++) {
    if (strcmp(tmp, IDENT_ARRAY[i]) == 0) {
      return i;
    }
  }
  
  IDENT_ARRAY[len] = tmp;
  return len++;
}

int find_ident(char* ident) {
  for (int i = 0; i < len; i++) {
    if (strcmp(ident, IDENT_ARRAY[i]) == 0) {
      return i;
    }
  }

  return -1;
}

char* tag_to_str(enum DOMAIN d) {
  switch (d)
  {
    case _EOF:
      return "EOF";
    case IDENT:
      return "IDENT";
    case WHILE:
      return "WHILE"; 
    case DO:
      return "DO";
    case END:
      return "END";
    case COMMENT:
      return "COMMENT"; 
    case NUMBER:
      return "NUMBER"; 
    default:
      return "UNDEFINED TAG"; 
  }
}


struct Position {
  int line, pos, index;
};

void print_pos(struct Position *p) { printf("(%d, %d)", p->line, p->pos); }

struct Fragment {
  struct Position starting, following;
};

typedef struct Fragment YYLTYPE;

int continued;
struct Position cur;

void print_frag(struct Fragment f) {
  print_pos(&(f.starting));
  printf(" - ");
  print_pos(&(f.following));
}

union Token {
  int ident;
};

#define YY_USER_ACTION                                                         \
  {                                                                            \
    int i;                                                                     \
    if (!continued)                                                            \
      yylloc->starting = cur;                                                  \
    continued = 0;                                                             \
    for (i = 0; i < yyleng; i++) {                                             \
      if (yytext[i] == '\n') {                                                 \
        ++cur.line;                                                            \
        cur.pos = 1;                                                           \
      } else {                                                                 \
        ++cur.pos;                                                             \
      }                                                                        \
      ++cur.index;                                                             \
    }                                                                          \
    yylloc->following = cur;                                                   \
  }

typedef union Token YYSTYPE;

void init_scanner(char *program) {
  continued = 0;
  cur.line = 1;
  cur.pos = 1;
  cur.index = 0;
  yy_scan_string(program);
}

void err(char *msg) {
  printf("Error");
  print_pos(&cur);
  printf(": %s\n ", msg);
}

%}

letter [a-zA-Z]
digit [0-9]
ident {letter}+
number {digit}*
comment \/\/\.*\n

%%

[\n\t ]+

"\/while\/"  {
              return WHILE;
             }

"\/do\/"     {
              return DO;
             }
"\/end\/"    {
              return END;
             }
{number}     {
              return NUMBER;
             }
{comment}    {
              return COMMENT;
             }
{ident}      {
              yylval->ident = add_ident(yytext);
              return IDENT;
             }
<<EOF>>      {
              return 0;
             }
%%


int main() {
  YYSTYPE value;
  YYLTYPE coords;
  int tag;
  
  do 
  {
    tag = yylex(&value, &coords);
    print_frag(coords);
    printf(" %s", tag_to_str(tag));
    if (tag == IDENT) {
      printf(" %d\n", value.ident);
    } else {
      printf("\n");
    }
  }
  while (tag);
  
  return 0;
}

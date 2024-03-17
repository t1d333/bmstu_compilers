% option noyywrap bison - bridge bison - locations


%{
#include < stdio.h>
#include < stdlib.h>

enum DOMAIN {
  IDENT,
  WHILE,
  DO,
  END,
  COMMENT,
}


void init_scanner ( char* program ) {
  continued = 0;
  cur.line = 1;
  cur.pos = 1;
  cur.index = 0;
  yy_scan_string(program);
}

struct Position {
  int line , pos , index ;
};

void print_pos(struct Position* p ) {
  printf ("(% d ,% d )" ,p - > line ,p - > pos );
}

struct Fragment {
  struct Position starting, following ;
};

typedef struct Fragment YYLTYPE ;

void print_frag(struct Fragment  f) {
  print_pos (&( f->starting));
  printf (" -");
  print_pos(&(f->following));
}


%}

IDENT [^\/]*
WHILE while
DO do
END end


%%

\/{IDENT}\/ {
  printf("ident");
}

\/{WHILE}\/ {
  printf("while");
}

\/{DO}\/ {
  printf("do");
}

\/{END}\/  {
  printf("end");
}


%%


# define PROGRAM ""

int main() {

  YYSTYPE value;
  YYLTYPE coords;
  init_scanner(PROGRAM);

  do
  {
    tag = yylex (&value ,&coords );
    if ( tag != 0) {
      
    }
  }
  while ( tag != 0);
return 0
}

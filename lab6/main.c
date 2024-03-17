#include <stdio.h>
#include <stdlib.h>

enum DOMAIN {
  IDENT,
  WHILE,
  DO,
  END,
  COMMENT,
};

void yy_scan_string(char *program);

struct Position {
  int line, pos, index;
};

void print_pos(struct Position *p) { printf("(% d ,% d )", p->line, p->pos); }

struct Fragment {
  struct Position starting, following;
};

typedef struct Fragment YYLTYPE;

int continued;
struct Position cur;

void print_frag(struct Fragment f) {
  print_pos(&(f.starting));
  printf(" -");
  print_pos(&(f.following));
}

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

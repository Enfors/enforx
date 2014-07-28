%{

#include <stdio.h>
#include "enforx.h"

%}

/* Declare tokens */
%token OPEN_PAREN CLOSE_PAREN
%token NUMBER

%%

program: { printf("program: (empty)\n"); } /* Nothing; matches at beginning of input */
| sexp_list        { printf("program: program sexp\n"); }
;

atom: NUMBER       { printf("atom: NUMBER\n"); }
;

sexp_list: sexp    { printf("sexp_list: sexp\n"); }
| sexp_list sexp   { printf("sexplist: sexp_list sexp\n"); }
;

sexp: atom          { printf("sexp: atom\n"); }
| OPEN_PAREN sexp_list CLOSE_PAREN { printf("sexp: OPEN_PAREN sexp_list CLOSE_PAREN\n"); }
;

%%

int main(int argc, char **argv)
{
  yyparse();
}

yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}

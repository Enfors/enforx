%{

#include <stdio.h>
#include "enforx.h"

  sexp_t *stack      = NULL;
  sexp_t *input_tree = NULL;

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

sexp: atom          { printf("sexp: atom\n"); }
| OPEN_PAREN sexp_list CLOSE_PAREN { printf("sexp: OPEN_PAREN sexp_list CLOSE_PAREN\n"); }
;

sexp_list: sexp    { printf("sexp_list: sexp\n"); }
| sexp_list sexp   { printf("sexplist: sexp_list sexp\n"); }
;

%%

int main(int argc, char **argv)
{
  input_tree = sexp_ref(sexp_new(CAR_SEXP, NULL));

  yyparse();
  printf("Foo.\n");

  sexp_unref(input_tree);

  enforx_end();
}

yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}

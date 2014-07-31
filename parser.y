%{

#include <stdio.h>
#include "enforx.h"

  sexp_t *stack      = NULL;
  sexp_t *root       = NULL;
  
  sexp_t *sexp       = NULL;

  sym_t  *atom       = NULL;

%}

/* Declare tokens */
%token OPEN_PAREN CLOSE_PAREN
%token NUMBER

%%

program: { printf("program: (empty)\n"); } /* Nothing; matches at beginning of input */
| sexp_list        { printf("program: program sexp\n"); }
;

atom: NUMBER       { 
  printf("atom: NUMBER\n");
  atom = sym_ref(sym_new(TYPE_NUM));
  sym_set_num(atom, yylval);
 }
;

open_paren: OPEN_PAREN {
  sexp_push(&stack, sexp_new(CAR_SEXP, NULL));
}
;

close_paren: CLOSE_PAREN {
  sexp_pop(&stack);
}
;

sexp: atom          { 
  printf("sexp: atom\n");
  sexp_append_child(stack, sexp_new(CAR_SYM, atom));
  sym_unref(atom);
 }
| open_paren sexp_list close_paren {
  printf("sexp: OPEN_PAREN sexp_list CLOSE_PAREN\n");  
 }
;

sexp_list: sexp    { printf("sexp_list: sexp\n"); }
| sexp_list sexp   { printf("sexplist: sexp_list sexp\n"); }
;

%%

int main(int argc, char **argv)
{
  root = sexp_ref(sexp_new(CAR_SEXP, NULL));
  sexp_push(&stack, root);

  yyparse();
  
  printf("Parsed input tree:\n");
  sexp_print_lisp_tree(root);
  printf("\n");

  /* sexp_pop(&stack); */
  sexp_unref(root);

  enforx_end();
}

yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}

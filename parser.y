%{

#include <stdio.h>
#include <string.h>
#include "enforx.h"

  sexp_t *stack      = NULL;
  sexp_t *root       = NULL;
  
  sym_t  *atom       = NULL;

  char   *str;

  int repl(void);

  extern int num_sexps;
  extern int num_syms;

%}

/* Declare tokens */
%token OPEN_PAREN CLOSE_PAREN
%token NUMBER STRING SYMBOL

%%

program: { /* printf("program: (empty)\n"); */ } /* Nothing; matches at beginning of input */
| sexp_list        { /* printf("program: program sexp\n"); */ }
;

atom: NUMBER       { 
  /* printf("atom: NUMBER\n"); */
  atom = sym_ref(sym_new(TYPE_NUM));
  sym_set_num(atom, yylval);
 }
| STRING           {
  /* str[strlen(str) - 1] = '\0'; */
  /* atom = sym_ref(sym_new(TYPE_STR)); */
  /* sym_set_str(atom, str); */
  /* printf("String! '%s'\n", str); */
  /* free(yylval.str); */
  }
;

open_paren: OPEN_PAREN {
  sexp_t *sexp = sexp_new(CAR_SEXP, NULL);
  sexp_append_child(sexp_stack_top(stack), sexp);
  sexp_push(&stack, sexp);
}
;

close_paren: CLOSE_PAREN {
  sexp_unref(sexp_pop(&stack));  
}
;

sexp: atom          { 
  sexp_append_child(sexp_stack_top(stack), sexp_new(CAR_SYM, atom));
  sym_unref(atom);
 }
| open_paren sexp_list close_paren {
  /* printf("sexp: OPEN_PAREN sexp_list CLOSE_PAREN\n");   */
 }
| open_paren close_paren
;

sexp_list: sexp    { /* printf("sexp_list: sexp\n"); */ }
| sexp_list sexp   { /* printf("sexplist: sexp_list sexp\n"); */ }
;

%%

int main(int argc, char **argv)
{
  repl();

  enforx_end();
}

int repl(void)
{
  /* REPL - Read, Eval, Print Loop */
  int i;

  while (1)
    {
      printf("Enforx-REPL> ");
      fflush(stdout);
      sexp_t *sexp = NULL;

      root = sexp_ref(sexp_new(CAR_SEXP, NULL));
      sexp_push(&stack, root);

      yyparse();

      while (sexp = sexp_pop(&stack))
	sexp_unref(sexp);

      sexp = root->car;

      sexp_print_lisp_tree(sexp);
      printf("\n");

      sexp_unref(root);

      if (num_sexps != 0)
	fprintf(stderr, "Warning: num_sexps = %d.\n", num_sexps);

      if (num_syms  != 0)
	fprintf(stderr, "Warning: num_syms = %d.\n",  num_syms);
    }

  return 0;
}

yyerror(char *s)
{
  fprintf(stderr, "error: %s\n", s);
}

#include <stdio.h>
#include <string.h>

#include "enforx.h"

void test_sym_print(void);
void test_sexp_print(void);

int main(int argc, char **argv)
{
  /*  test_sym_print(); */

  test_sexp_print();
  
  return 0;
}

void test_sym_print(void)
{
  sym_t *sym = sym_new(TYPE_NUM);
  
  sym_set_num(sym, 42);
  sym_print(sym);
  printf("\n");

  sym_set_str(sym, strdup("Hello world!"));
  sym_print(sym);
  printf("\n");

  sym_clear(sym);
  sym_print(sym);
  printf("\n");
}

void test_sexp_print()
{
  sexp_t *sexp1       = sexp_new(CAR_SEXP,     NULL);
  sexp_t *sexp1_1     = sexp_new(  CAR_SYM,    sym_new(TYPE_NUM));
  sexp_t *sexp1_2     = sexp_new(  CAR_SEXP,   NULL);
  sexp_t *sexp1_2_1   = sexp_new(    CAR_SYM,  sym_new(TYPE_NUM));
  sexp_t *sexp1_2_2   = sexp_new(    CAR_SYM,  sym_new(TYPE_STR));
  sexp_t *sexp1_3     = sexp_new(  CAR_SYM,    sym_new(TYPE_NUM));

  sexp_set_car(sexp1,         sexp1_1);
  sexp_set_cdr(  sexp1_1,     sexp1_2);
  sexp_set_car(  sexp1_2,     sexp1_2_1);
  sexp_set_cdr(    sexp1_2_1, sexp1_2_2);
  sexp_set_cdr(  sexp1_2,     sexp1_3);

  sym_set_num(sexp1_1->sym,   1);
  sym_set_num(sexp1_2_1->sym, 2);
  sym_set_str(sexp1_2_2->sym, "foo");
  sym_set_num(sexp1_3->sym,   3);

  printf("sexp_print_tree():\n");
  sexp_print_tree(sexp1, 0);

  printf("\nsexp_print_lisp_tree():\n");
  sexp_print_lisp_tree(sexp1);
  printf("\n");
}

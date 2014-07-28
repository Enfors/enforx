#include <stdio.h>
#include <string.h>

#include "enforx.h"

void test_sexp_print(void);

int main(int argc, char **argv)
{
  test_sexp_print();
  
  return 0;
}

void test_sexp_print(void)
{
  sexp_t *sexp = sexp_new(TYPE_NUM);
  
  sexp_set_num(sexp, 42);
  sexp_print(sexp);
  printf("\n");

  sexp_set_str(sexp, strdup("Hello world!"));
  sexp_print(sexp);
  printf("\n");

  sexp_clear(sexp);
  sexp_print(sexp);
  printf("\n");
}

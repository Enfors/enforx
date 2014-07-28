#include <stdio.h>
#include <string.h>

#include "enforx.h"

void test_sym_print(void);

int main(int argc, char **argv)
{
  test_sym_print();
  
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

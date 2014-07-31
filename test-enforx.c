#include <stdio.h>
#include <string.h>

#include "enforx.h"

void test_sym_print(void);
void test_sexp_print(void);
void test_garbage_collection(void);
void test_lists(void);
void test_stack(void);

extern long num_sexps;
extern long num_syms;

int main(int argc, char **argv)
{
  /* test_sym_print(); */

  test_sexp_print();

  test_garbage_collection();

  test_lists();

  test_stack();
  
  enforx_end();

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
  sexp_t *sexp1       = sexp_ref(sexp_new(CAR_SEXP,     NULL));
  sexp_t *sexp1_1     =          sexp_new(  CAR_SYM,    sym_new(TYPE_NUM));
  sexp_t *sexp1_2     =          sexp_new(  CAR_SEXP,   NULL);
  sexp_t *sexp1_2_1   =          sexp_new(    CAR_SYM,  sym_new(TYPE_NUM));
  sexp_t *sexp1_2_2   =          sexp_new(    CAR_SYM,  sym_new(TYPE_STR));
  sexp_t *sexp1_3     =          sexp_new(  CAR_SYM,    sym_new(TYPE_NUM));

  printf("\n==== test_sexp_print ====\n");

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

  sexp_unref(sexp1);
}

void test_garbage_collection(void)
{
  /* 
   * This function tests the garbage collection facilities.  When you
   * use a pointer to a sexp or sym, you first have to call its
   * corresponding _ref() function to increase its reference count. In
   * this case, we only have to do that on the first one, because the
   * others will have their _ref() called when we set them as car or
   * cdr of the first one.  If we unref (and thereby decrease
   * ref_count to 0) one of the sexps, that one and all the ones
   * following it will be unallocated, because there's no longer
   * anything pointing to it. And when we unref the first one (sexp1),
   * everything else will be unallocated for the same reason.
   */
  sexp_t *sexp1     = sexp_ref(sexp_new(CAR_SEXP,   NULL));
  sexp_t *sexp1_1   =          sexp_new(  CAR_SYM,  sym_new(TYPE_NUM));
  sexp_t *sexp1_2   =          sexp_new(  CAR_SYM,  sym_new(TYPE_NUM));
  sexp_t *sexp1_3   =          sexp_new(  CAR_SEXP, NULL);
  sexp_t *sexp1_3_1 =          sexp_new(  CAR_SYM,  sym_new(TYPE_NUM));
  sexp_t *sexp1_3_2 =          sexp_new(  CAR_SEXP, NULL);

  printf("\n==== test_garbage_collection() ====\n");

  /* Construct a tree looking like "(0 0 (0))". */
  sexp_set_car(sexp1, sexp1_1);

  sexp_set_cdr(sexp1_1, sexp1_2);
  sexp_set_cdr(sexp1_2, sexp1_3);
  
  sexp_set_car(sexp1_3, sexp1_3_1);
  sexp_set_cdr(sexp1_3_1, sexp1_3_2);

  /* Set some symbol values - '(1 2 ("three") ())' */
  sym_set_num(sexp1_1->sym,   1);
  sym_set_num(sexp1_2->sym,   2);
  sym_set_str(sexp1_3_1->sym, "three");

  /* Print it, to make sure it looks right. */
  sexp_print_lisp_tree(sexp1);
  printf("\nnum_sexps: %d, num_syms: %d\n", num_sexps, num_syms);

  /* Remove the cdr pointer to one of them in the middle of the list.
   * This means that the ones following it become unreferenced, and
   * they are unallocated. */
  sexp_set_cdr(sexp1_2, NULL);

  /* Print again, the list should look like "(1 2)" now. */
  sexp_print_lisp_tree(sexp1);
  printf("\nnum_sexps: %d, num_syms: %d\n", num_sexps, num_syms);

  /* This should unravel the remaining parts, leaving nothing behind. */
  sexp1 = sexp_unref(sexp1);
}

void test_lists(void)
{
  sexp_t *root     = sexp_ref(sexp_new(CAR_SEXP, NULL));
  sexp_t *list     =          sexp_new(CAR_SYM,  sym_new(TYPE_NUM));
  sexp_t *appendee =          sexp_new(CAR_SYM,  sym_new(TYPE_NUM));

  printf("\n==== test_lists() ====\n");

  sexp_set_car(root, list);
  printf("Before append:\n");
  sexp_print_lisp_tree(root);
  printf("\n");

  sexp_append(list, appendee);
  printf("After append:\n");
  sexp_print_lisp_tree(root);
  printf("\n");

  sexp_unref(root);
}

void test_stack(void)
{
  sexp_t *stack = NULL;
  sexp_t *sexp1 = sexp_ref(sexp_new(CAR_SYM, sym_new(TYPE_NUM)));
  sexp_t *sexp2 = sexp_ref(sexp_new(CAR_SYM, sym_new(TYPE_NUM)));
  sexp_t *sexp3 = sexp_ref(sexp_new(CAR_SYM, sym_new(TYPE_NUM)));
  sexp_t *sexp  = NULL;

  printf("\n==== test_stack() ====\n");

  sym_set_num(sexp1->sym, 1001);
  sym_set_num(sexp2->sym, 2002);
  sym_set_num(sexp3->sym, 3003);

  sexp_push(&stack, sexp1);
  sexp1 = sexp_unref(sexp1);

  sexp_push(&stack, sexp2);
  sexp2 = sexp_unref(sexp2);

  sexp_push(&stack, sexp3);
  sexp3 = sexp_unref(sexp3);

  while (sexp = sexp_pop(&stack))
    {
      sym_print(sexp->sym);
      printf("\n");
      sexp_unref(sexp);
    }
}

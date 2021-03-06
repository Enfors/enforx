/* enforx.c by Christer Enfors, 2014. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "enforx.h"

long num_sexps = 0;
long num_syms  = 0;

/*
 * sexp functions
 */

sym_t *sym_new(unsigned const int type)
{
  sym_t *sym = Malloc(sizeof(sym_t));

  sym->type = type;

  switch (type)
    {
    case TYPE_NUM:
      sym->num = 0;
      break;

    case TYPE_STR:
      sym->str = NULL;
      break;

    default:
      fprintf(stderr, "Error: sym_new: unsupported type '%d'.\n", type);
      break;
    }

  sym->ref_count = 0;

  num_syms++;

  return sym;
}

void sym_del(sym_t *sym)
{
  assert(sym != NULL);
  
  switch (sym->type)
    {
    case TYPE_NUM:
      break;
      
    case TYPE_STR:
      if (sym->str)
	Free(sym->str);
      break;

    default:
      fprintf(stderr, "Error: sym_del(): unsupported type '%d'.\n", sym->type);
      break;
    }
  
  Free(sym);

  num_syms--;
}

sym_t *sym_ref(sym_t *sym)
{
  if (sym == NULL)
    return NULL;

  sym->ref_count++;

  return sym;
}

sym_t *sym_unref(sym_t *sym)
{
  if (sym == NULL)
    return NULL;

  sym->ref_count--;
  
  if (sym->ref_count < 0)
    {
      fprintf(stderr, "Warning: sym_unref(): ref_count is %d. Symbol:\n", sym->ref_count);
      sym_print(sym);
    }

  if (sym->ref_count == 0)
    sym_del(sym);

  return NULL;
}

void sym_print(sym_t *sym)
{
  if (sym == NULL)
    printf("NULL sym");

  switch (sym->type)
    {
    case TYPE_NUM:
      printf("%d", sym->num);
      break;

    case TYPE_STR:
      if (sym->str)
	printf("\"%s\"", sym->str);
      else
	printf("\"\"");
      break;

    default:
      fprintf(stderr, "Error: sym_print: unsupported type '%d'.\n", sym->type);
      break;
    }
  /* printf(" (ref_count: %d)", sym->ref_count); */
}

void sym_clear(sym_t *sym)
{
  assert(sym != NULL);

  switch (sym->type)
    {
    case TYPE_NUM:
      sym->num = 0;
      break;

    case TYPE_STR:
      if (sym->str)
	Free(sym->str);
      sym->str = NULL;
      break;

    default:
      fprintf(stderr, "Error: sym_clear: unsupported type '%d'.\n", sym->type);
      break;
    }
}

void sym_set_num(sym_t *sym, long num)
{
  sym_clear(sym);

  sym->type = TYPE_NUM;
  sym->num  = num;
}

void sym_set_str(sym_t *sym, char *str)
{
  sym_clear(sym);

  sym->type = TYPE_STR;
  sym->str  = Strdup(str);
}
 
/*
 *
 * sexp functions.
 *
 */

sexp_t *sexp_new(unsigned const int type, sym_t *sym)
{
  sexp_t *sexp = Malloc(sizeof(sexp_t));

  sexp->type = type;

  switch (type)
    {
    case CAR_SYM:
      sexp->sym = sym_ref(sym);
      break;

    case CAR_SEXP:
      sexp->car = NULL;
      break;

    default:
      fprintf(stderr, "Error: sexp_new: unsupported type '%d'.\n", type);
      break;
    }

  sexp->ref_count = 0;

  num_sexps++;

  return sexp;
}

void sexp_del(sexp_t *sexp)
{
  assert(sexp != NULL);

  switch (sexp->type)
    {
    case CAR_SYM:
      assert(sexp->sym != NULL);
      sexp->sym = sym_unref(sexp->sym);
      break;

    case CAR_SEXP:
      if (sexp->car) 
	{
	  sexp->car = sexp_unref(sexp->car);
	}
      break;

    default:
      fprintf(stderr, "Error: sexp_del: unsupported type '%d'.\n", sexp->type);
      break;
    }

  Free(sexp);

  sexp->cdr = sexp_unref(sexp->cdr);

  num_sexps--;
}

sexp_t *sexp_ref(sexp_t *sexp)
{
  if (sexp == NULL)
    return NULL;

  sexp->ref_count++;

  return sexp;
}

sexp_t *sexp_unref(sexp_t *sexp)
{
  if (! sexp)
    return NULL;

  sexp->ref_count--;

  if (sexp->ref_count < 0)
    {
      fprintf(stderr, "Warning: sexp_unref(): ref_count is %d. "
	      "Sexp: %x.\n", sexp->ref_count, sexp);
    }
  
  if (sexp->ref_count == 0)
    sexp_del(sexp);

  return NULL;
}
 
/*void sexp_set_car_sym(sexp_t *sexp, sym_t *sym)
{
  assert(sexp != NULL);

  sexp->type = CAR_SYM;
  sym_ref(sym);
  sexp->sym = sym;
}

void sexp_set_car_sexp(sexp_t *sexp, sexp_t *car)
{
  assert(sexp != NULL);

  sexp->type = CAR_SEXP;
  sexp_ref(car);
  sexp->car = sexp;
  }*/

void sexp_print(sexp_t *sexp)
{
  assert(sexp != NULL);

  switch (sexp->type)
    {
    case CAR_SYM:
      sym_print(sexp->sym);
      printf("\n");
      break;

    case CAR_SEXP:
      printf("Parent:\n");
      break;

    default:
      fprintf(stderr, "Error: sexp_print: unsupported type '%d'.\n", sexp->type);
      break;
    }
}

void sexp_print_tree(sexp_t *sexp, int indent)
{
  int i = 0;

  assert(sexp != NULL);

  while (sexp)
    {
      for (i = 0; i < indent * 2; i++)
	printf(" ");

      sexp_print(sexp);

      if (sexp->type == CAR_SEXP)
	sexp_print_tree(sexp->car, indent + 1);

      sexp = sexp->cdr;
    }
}

void sexp_print_lisp_tree(sexp_t *sexp)
{
  int extra_space = 0;

  if (sexp == NULL)
    return;

  while (sexp)
    {
      if (extra_space)
	printf(" ");
      else
	extra_space = 1;
      
      switch (sexp->type)
	{
	case CAR_SEXP:
	  printf("(");
	  sexp_print_lisp_tree(sexp->car);
	  printf(")");
	  break;

	case CAR_SYM:
	  sym_print(sexp->sym);
	  
	  /* if (sexp->cdr) */
	  /*   printf(" "); */

	  break;

	default:
	  fprintf(stderr, "Error: sexp_print_lisp_tree: Unsupported type '%d'.\n", sexp->type);
	  break;
	}

      sexp = sexp->cdr;
    }
}

void sexp_clear(sexp_t *sexp)
{
  assert(sexp != NULL);

  switch (sexp->type)
    {
    case CAR_SYM:
      assert(sexp->sym != NULL);
      sym_unref(sexp->sym);
      sexp->sym = NULL;
      break;

    case CAR_SEXP:
      if (sexp->car == NULL)
	return;

      sexp_unref(sexp->car);
      sexp->car = NULL;
    }
}

void sexp_set_sym(sexp_t *sexp, sym_t *sym)
{
  assert(sexp != NULL);

  /* Unref the old car or sym */
  switch (sexp->type)
    {
    case CAR_SYM:
      sexp->sym = sym_unref(sexp->sym);
      break;

    case CAR_SEXP:
      sexp->car = sexp_unref(sexp->car);
      break;

    default:
      fprintf(stderr, "Error: sexp_set_sym(): Unsupported type '%d'.\n", sexp->type);
      break;
    }

  sexp->type = CAR_SYM;
  sexp->sym  = sym_ref(sym);
}

void sexp_set_car(sexp_t *parent_sexp, sexp_t *car_sexp)
{
  assert(parent_sexp != NULL);

  sexp_clear(parent_sexp);
  
  parent_sexp->type = CAR_SEXP;
  parent_sexp->car  = sexp_ref(car_sexp);
}

void sexp_set_cdr(sexp_t *sexp, sexp_t *cdr_sexp)
{
  assert(sexp != NULL);

  sexp->cdr = sexp_unref(sexp->cdr);

  sexp->cdr = sexp_ref(cdr_sexp);
}

void sexp_append(sexp_t *list, sexp_t *sexp)
{
  sexp_t *cur_sexp = list;

  assert(sexp != NULL);
  assert(list != NULL);
  
  while (cur_sexp->cdr)
    cur_sexp = cur_sexp->cdr;

  sexp_set_cdr(cur_sexp, sexp);
}

void sexp_append_child(sexp_t *parent, sexp_t *child)
{
  assert(parent != NULL);
  assert(child  != NULL);
  assert(parent->type == CAR_SEXP);

  if (parent->car == NULL)
    sexp_set_car(parent, child);
  else
    sexp_append(parent->car, child);
}

/*
 * The stack makes containers for each item it contains.
 */
void sexp_push(sexp_t **stack, sexp_t *sexp)
{
  sexp_t *container = sexp_ref(sexp_new(CAR_SEXP, NULL));

  assert(sexp != NULL);

  sexp_set_car(container, sexp);

  container->cdr = sexp_ref(*stack);

  *stack = container;
}

/*
 * Each item coming out of the stack will have one ref set to it by the stack
 * (and perhaps other refs set elsewhere). The one who calls sexp_pop() must
 * unref this ref.
 */
sexp_t *sexp_pop(sexp_t **stack)
{
  sexp_t *container, *sexp;

  if (*stack == NULL)
    return NULL;

  container = *stack;
  sexp = sexp_ref(container->car);
  *stack = container->cdr; /* Have stack point to the next one (no ref). */
  sexp_unref(container); /* Container no longer needed, let it be deleted. */

  return sexp;
}

sexp_t *sexp_stack_top(sexp_t *stack)
{
  if (stack == NULL)
    return NULL;
  else
    {
      assert(stack->car != NULL);
      return (sexp_t *) stack->car;
    }
}

/*
 * Misc functions
 */

void enforx_end(void)
{
  if (num_sexps != 0)
    {
      fprintf(stderr, "Warning: enforx_end(): num_sexps is %d.\n", num_sexps);
    }

  if (num_syms != 0)
    {
      fprintf(stderr, "Warning: enforx_end(): num_syms is %d.\n", num_syms);
    }
}

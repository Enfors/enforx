/* enforx.h by Christer Enfors, 2014. */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "enforx.h"

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

  return sym;
}

void sym_del(sym_t *sym)
{
  sym_t *cdr;
  
  if (sym == NULL)
    return;
  
  switch (sym->type)
    {
    case TYPE_NUM:
      break;
      
    case TYPE_STR:
      if (sym->str)
	Free(sym->str);
      break;

    default:
      fprintf(stderr, "Error: sym_del: unsupported type '%d'.\n", sym->type);
      break;
    }
  
  Free(sym);
}

sym_t *sym_ref(sym_t *sym)
{
  /* todo: increase ref count. */
  return sym;
}

sym_t *sym_unref(sym_t *sym)
{
  /* todo: decrease ref count. */
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
  sym->str  = str;
}
 
/*
 *
 * sexp functions.
 *
 */

sexp_t *sexp_ref(sexp_t *sexp)
{
  /* todo: increase ref count */
  return sexp;
}

sexp_t *sexp_unref(sexp_t *sexp)
{
  /* todo: decrease ref count */
  return NULL;
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
  sexp_clear(sexp);

  sexp->type = CAR_SYM;
  sexp->sym  = sym_ref(sym);
}

void sexp_set_car(sexp_t *parent_sexp, sexp_t *car_sexp)
{
  sexp_clear(parent_sexp);
  
  parent_sexp->type = CAR_SEXP;
  parent_sexp->car  = sexp_ref(car_sexp);
}

sexp_t *sexp_push(sexp_t *stack, sexp_t *sexp)
{
  assert(sexp != NULL);

  sexp->cdr = sexp_ref(stack);

  return sexp;
}

/* The following function would be more useful if it didn't suck.
   It should take a ** (or something) to the stack, so that it
   could be modified in-place to point to the new top. But I don't
   know how to accomplish that right now. */
sexp_t *sexp_pop(sexp_t *stack)
{
  sexp_t *sexp;

  if (stack == NULL)
    return NULL;

  sexp = sexp_ref(stack);
  sexp->cdr = NULL;

  return sexp;
}

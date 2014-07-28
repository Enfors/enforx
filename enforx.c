/* enforx.h by Christer Enfors, 2014. */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "enforx.h"

/*
 * sexp functions
 */

sexp_t *sexp_new(unsigned const int type)
{
  sexp_t *sexp = Malloc(sizeof(sexp_t));

  sexp->type = type;

  switch (type)
    {
    case TYPE_NUM:
      sexp->num = 0;
      break;

    case TYPE_STR:
      sexp->str = NULL;
      break;

    case TYPE_SYM:
      sexp->sym = NULL;
      break;

    case TYPE_PARENT:
      sexp->child = NULL;
      break;
    }

  return sexp;
}

sexp_t *sexp_del(sexp_t *sexp)
{
  sexp_t *cdr;

  if (sexp == NULL)
    return NULL;

  cdr = sexp->cdr;

  Free(sexp);

  return cdr;
}

void sexp_print(sexp_t *sexp)
{
  if (sexp == NULL)
    printf("NULL sexp");

  switch (sexp->type)
    {
    case TYPE_NUM:
      printf("%d", sexp->num);
      break;

    case TYPE_STR:
      if (sexp->str)
	printf("\"%s\"", sexp->str);
      else
	printf("\"\"");
      break;

    default:
      fprintf(stderr, "Error: sexp_print: unsupported type '%d'.\n", sexp->type);
      break;
    }
}

void sexp_clear(sexp_t *sexp)
{
  assert(sexp != NULL);

  switch (sexp->type)
    {
    case TYPE_NUM:
      sexp->num = 0;
      break;

    case TYPE_STR:
      if (sexp->str)
	Free(sexp->str);
      sexp->str = NULL;
      break;

    case TYPE_SYM:
      if (sexp->sym)
	{
	  Free(sexp->sym);
	  fprintf(stderr, "Warning: sexp_clear: should probably call a sym_del() function here.\n");
	}
      sexp->sym = NULL;
      break;

    case TYPE_PARENT:
      if (sexp->child)
	Free(sexp->child);
      sexp->child = NULL;
      break;
    }
}

void sexp_set_num(sexp_t *sexp, long num)
{
  sexp_clear(sexp);

  sexp->type = TYPE_NUM;
  sexp->num  = num;
}

void sexp_set_str(sexp_t *sexp, char *str)
{
  sexp_clear(sexp);

  sexp->type = TYPE_STR;
  sexp->str  = str;
}

void sexp_set_sym(sexp_t *sexp, symbol_t *sym)
{
  sexp_clear(sexp);

  sexp->type = TYPE_SYM;
  sexp->sym  = sym;
}

void sexp_set_child(sexp_t *parent_sexp, sexp_t *child_sexp)
{
  sexp_clear(parent_sexp);
  
  parent_sexp->type  = TYPE_PARENT;
  parent_sexp->child = child_sexp;
}

sexp_t *sexp_push(sexp_t *stack, sexp_t *sexp)
{
  if (sexp == NULL)
    {
      fprintf(stderr, "Error: sexp_push(): sexp is NULL\n");
      exit(1);
    }

  sexp->cdr = stack;

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

  sexp = stack;
  sexp->cdr = NULL;

  return sexp;
}

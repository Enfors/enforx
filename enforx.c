/* enforx.h by Christer Enfors, 2014. */

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
    case TYPE_LONG:
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

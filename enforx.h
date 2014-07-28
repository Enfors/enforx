#define MAX_SYMBOL_NAME_SIZE   80

#ifndef NULL
#  define  NULL 0
#endif

typedef struct symbol_t
{
  char name[MAX_SYMBOL_NAME_SIZE];
  
  union
  {
    long     num;
    char    *str;
  };

} symbol_t;

enum { TYPE_NUM, TYPE_STR, TYPE_SYM, TYPE_PARENT };

typedef struct sexp_t
{
  unsigned int type;
  
  /* CAR */
  union
  {
    long      num;
    char     *str;
    symbol_t *sym;
    struct sexp_t   *child;
  };
    
  struct sexp_t *cdr;

} sexp_t;

/*
 * Function declarations.
 */

sexp_t *sexp_new(unsigned const int type);
sexp_t *sexp_del(sexp_t *sexp);

void    sexp_print(sexp_t *sexp);

void    sexp_clear(sexp_t *sexp);
void    sexp_set_num(sexp_t *sexp, long num);
void    sexp_set_str(sexp_t *sexp, char *str);
void    sexp_set_sym(sexp_t *sexp, symbol_t *sym);
void    sexp_set_child(sexp_t *parent_sexp, sexp_t *child_sexp);

sexp_t *sexp_push(sexp_t *stack, sexp_t *sexp);
sexp_t *sexp_pop(sexp_t  *stack);

/*
 * Macros meant to be replaced by functions at a later date.
 */

#define Malloc(x)        malloc(x)
#define Free(x)          free(x)

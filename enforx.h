#define MAX_SYMBOL_NAME_SIZE   80

typedef struct symbol_t
{
  char name[MAX_SYMBOL_NAME_SIZE];
  
  union
  {
    long     num;
    char    *str;
  };

} symbol_t;

enum { TYPE_LONG, TYPE_STR, TYPE_SYM, TYPE_PARENT };

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

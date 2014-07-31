#define MAX_SYMBOL_NAME_SIZE   80

#ifndef NULL
#  define  NULL 0
#endif

enum { TYPE_NUM, TYPE_STR };

/*
 *
 * sym_t
 *
 */

typedef struct sym_t
{
  /* A symbol can be an actual LISP style symbol - that is, a value with
   * an attached name, as in (setq my-symbol 1). But it can also be an
   * "anonymous" value (without a name). For example, in (setq foo 1) the
   * "1" is an anonymous symbol.
   */
  unsigned int type; /* TYPE_NUM, TYPE_STR */
  char name[MAX_SYMBOL_NAME_SIZE];
  
  union
  {
    long     num;
    char    *str;
  };

  int ref_count;

} sym_t;

 /*
  *
  * sexp_t
  *
  */

enum { CAR_SYM, CAR_SEXP };

typedef struct sexp_t
{
  /* A sexp can be one of two types. CAR_SYM means that its car is a a
   * symbol (anonymous, IE lacking a name, or otherwise), while
   * CAR_SEXP means that it has another sexp as a car.
   *
   * CAR_SYM:              | CAR_SEXP:   
   * ========              | =========
   *                       |
   * +------+              | +------+
   * | SEXP |--> NULL      | | SEXP |--> NULL
   * +------+              | +------+
   *    |                  |    |
   *    V                  |    V
   * +------+              | +------+
   * | SYM  |              | | SEXP |--> NULL
   * +------+              | +------+
   *                       |    |
   *                       |    V
   *                       |   NULL
   */
  unsigned int type;
  
  /* CAR */
  union 
  {
    sym_t      *sym;
    struct sexp_t *car;
  };
  
  struct sexp_t *cdr;
  
  int ref_count;
  
} sexp_t;

/*
 * Function declarations.
 */

sym_t  *sym_new(unsigned const int type);
void    sym_del(sym_t *sym);

sym_t  *sym_ref(sym_t *sym);
sym_t  *sym_unref(sym_t *sym);

void    sym_print(sym_t *sym);

void    sym_clear(sym_t *sym);
void    sym_set_num(sym_t *sym, long num);
void    sym_set_str(sym_t *sym, char *str);

sexp_t *sexp_new(unsigned const int type, sym_t *sym);
void    sexp_del(sexp_t *sexp);

sexp_t *sexp_ref(sexp_t *sexp);
sexp_t *sexp_unref(sexp_t *sexp);

/*
  void    sexp_set_car_sym(sexp_t *sexp,  sym_t  *sym);
  void    sexp_set_car_sexp(sexp_t *sexp, sexp_t *car);
*/

void    sexp_print(sexp_t *sexp);
void    sexp_print_tree(sexp_t *sexp, int indent);
void    sexp_print_lisp_tree(sexp_t *sexp);
void    sexp_clear(sexp_t *sexp);

void    sexp_set_sym(sexp_t *sexp, sym_t *sym);
void    sexp_set_car(sexp_t *parent_sexp, sexp_t *car_sexp);

void    sexp_set_cdr(sexp_t *sexp, sexp_t *cdr_sexp);

/* Append sexp at the very end of list. Return list. */
void    sexp_append(sexp_t *list, sexp_t *sexp);
void    sexp_append_child(sexp_t *parent, sexp_t *child);

void    sexp_push(sexp_t **stack, sexp_t *sexp);
sexp_t *sexp_pop(sexp_t **stack);

void    enforx_end(void);


/*
 * Macros meant to be replaced by functions at a later date.
 */

#define Malloc(x)        malloc(x)
#define Free(x)          free(x)
#define Strdup(x)        strdup(x)

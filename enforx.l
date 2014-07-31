%{

#include "parser.tab.h"

%}

%%

"("        { return OPEN_PAREN; }
")"        { return CLOSE_PAREN; }
[0-9]+     { yylval = atoi(yytext); return NUMBER; }
[ \t\r\n]  { /* Ignore whitespace */ }

%{

#include "parser.tab.h"

%}

%%

"("        { return OPEN_PAREN; }
")"        { return CLOSE_PAREN; }
[0-9]+     { yylval = atoi(yytext); return NUMBER; }
\"[^"]\"   { /* yylval.str = strdup(yytext); */ return STRING; }
[ \t\r\n]  { /* Ignore whitespace */ }

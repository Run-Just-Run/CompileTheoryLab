%option noyywrap

%{
#include "cal.h"
%}




%%
"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")" { return yytext[0]; }

\.?[0-9]+|[0-9]+\.[0-9]* { yylval.d = atof(yytext); return NUMBER; }

\n          { return EOL; }
"//".*\n
[ \t]       { /* ºöÂÔ¿Õ°××Ö·û */}
.           { yyerror("Mystery character %c\n"); }
%%

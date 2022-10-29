/*test.l*/
%{
#include <stdio.h>
#include "cal.h"
void yyerror(char *);
%}
NUM \.?[0-9]+|[0-9]+\.[0-9]*
%%

{NUM}		                {yylval=yytext;return NUM;}
[a-zA-Z_$]+[a-zA-Z_$0-9]*	{yylval=yytext;return VAR;}
[-+()=]	{return *yytext;}
\n                          return CR;
[ \t]+                     /* ignore whitespace */;
.
%%

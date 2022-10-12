%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>
	int line=1;
%}
LETTER [a-zA-Z]
ID ({LETTER}|_)({LETTER}|{DIGIT})* 
DIGIT [0-9]

%%
\n {++line;}
{DIGIT} {printf("line%d:(integer, %s)\n",line,yytext);} 
{ID} {printf("line%d:(Ident, %s)\n",line,yytext);}
[ \t]+ {}

%%
int main(){
	yyin=fopen("C:\\Users\\86133\\Desktop\\win_flex_bison-latest\\test.txt","r");
	yylex();
	getchar();
	return 0;
}
int yywrap(){
	return 1;
}


/*test.y*/
%{
#include <stdio.h>
#include <string.h>
#define YYSTYPE char*
int yylex(void);
void yyerror(char *);
%}

%token NUM VAR CR

%%

				program:
    program stat CR 
    |
    
    ;
    stat:
    expr {printf("%s ", $1);printf("YES\n");}
    |VAR '=' expr;
expr:
    NUM
	
    |VAR
    |expr '+' expr    
    |expr '-' expr    
    |expr '*' expr    
    |expr '/' expr
    |'('expr')'   ;  

%%
void yyerror(char *str){
    fprintf(stderr,"error:%s\n",str);
}

int yywrap(){
    return 1;
}
int main()
{   extern FILE* yyin;
    yyin=fopen("C:\\Users\\86133\\Desktop\\text.txt","r");
    while(!feof(yyin)){
    yyparse(); 
    yyin++;
    }
    system("pause");
}

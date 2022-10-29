/* file: 3.02_create_AST_with_bison.y */

%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(char* str) {
    fprintf(stderr, "error:%s\n", str);
}
enum NodeT { // ���������﷨���ڵ����͵�ö��ֵ
	NT_UNDEF,
	NT_ADD,
	NT_SUB,
	NT_MUL,
	NT_DIV,
	NT_ABS,
	NT_NEG,
	NT_NUM,
};

/* �����﷨���еĽڵ� */
struct ast {
	int nodetype;
	struct ast* lft;
	struct ast* rht;
};

struct numval {
	int nodetype; /* ����K �������� */
	double number;
};

/* Ϊ������������һ������ */
struct ast* newast(int nodetype, struct ast* lft, struct ast* rht) {
    struct ast* a = malloc(sizeof(struct ast));
    if (!a) {
        yyerror("out of memory!");
        exit(0);
    }
    a->nodetype = nodetype;
    a->lft = lft;
    a->rht = rht;
    return a;
}

/* Ϊһ��ֵ������һ��Ҷ�ӽڵ� */
struct ast* newnum(double d) {
    struct numval* a = malloc(sizeof(struct numval));
    if (!a) {
        yyerror("out of memory!");
        exit(0);
    }
    a->nodetype = NT_NUM;
    a->number = d;

    // �˴���"struct numval"������ǿ��ת��Ϊ"struct ast"���ͣ�����Ϊ�ܹ�ͳһ����eval������������ͣ�
    // ͨ��nodetype�ֶ�ʶ��"struct numval"���͵�ʵ��ʱ��Ҫʹ�ø�ʵ����Ҫǿ��ת���������С�
    return (struct ast*)a;
}

/* �ݹ�������������ֵ */
double eval(struct ast* a) {
    double v = 0; // result of eval

    switch (a->nodetype) {
    case NT_NUM:
        v = ((struct numval*)a)->number;
        break;
    case NT_ADD:
        v = eval(a->lft) + eval(a->rht);
        break;
    case NT_SUB:
        v = eval(a->lft) - eval(a->rht);
        break;
    case NT_MUL:
        v = eval(a->lft) * eval(a->rht);
        break;
    case NT_DIV:
        v = eval(a->lft) / eval(a->rht);
        break;
    case NT_ABS:
        v = eval(a->lft);
        v = v < 0 ? v * -1.0f : v;
        break;
    case NT_NEG: // ��������ֵ
        v = eval(a->lft) * -1.0f;
        break;

    default:
        printf("internal error:bad nodetype=%c,%d\n",
            a->nodetype, a->nodetype);
        break;
    }
    return v;
}

/* �ݹ��ͷŸ������ڵ� */
void treefree(struct ast* a) {
    switch (a->nodetype) {
    case NT_ADD:/* 2������ */
    case NT_SUB:
    case NT_MUL:
    case NT_DIV:
        treefree(a->lft);
        treefree(a->rht);
        break;
    case NT_ABS: /* 1������ */
    case NT_NEG:
        treefree(a->lft);
        break;
    case NT_NUM:/* û������ */
        free(a);
        a = NULL;
        break;
    default:
        printf("internal error:bad nodetype=%c,%d\n",
            a->nodetype, a->nodetype);
        break;
    }
}

%}

/* %union�ؼ����������﷨�����������﷨ֵ���ܻ��õ����������͵�һ������ */
%union {
  struct ast *a;
  double d;
}

%token <d> NUMBER /* �����Ǻţ���"<d>"ָ���˸üǺŵ��������� */
%token EOL

/* ��ʹ����union������һ���������ͼ��Ϻ������Ϊ���еķ��ս��ָ����ֵ���͡�ʹ��"%type"�ؼ���*/
%type <a> exp factor term

%start calclist /* ָ����ʼ���ţ�start symbol����ʱҲ��ΪĿ����ţ�goal symbol�� */

%%
calclist: /* -- �չ�����ʼ���ű���߱�һ���չ���ּ������ʼ���ű���ƥ���������� -- */
        | calclist exp EOL { printf("The expression is legal,the result is:\n");printf("= %4.4g\n", eval($2)); treefree($2); printf("> "); }
        | calclist EOL { printf("> "); } /* ���л�ע�� */
        ;

exp: factor
   | exp '+' factor { $$ = newast(NT_ADD, $1, $3); }
   | exp '-' factor { $$ = newast(NT_SUB, $1, $3); }
   ;

factor: term
      | factor '*' term { $$ = newast(NT_MUL, $1, $3); }
      | factor '/' term { $$ = newast(NT_DIV, $1, $3); }
      ;

term: NUMBER { $$ = newnum($1); }
    | '|' exp '|' { $$ = newast(NT_ABS, $2, NULL); }     // ʹ�þ���ֵ���ŵ��﷨����
    | '(' exp ')' { $$ = $2; }                           // ʹ��Բ���ŵ��﷨����
    | '-' term    { $$ = newast(NT_NEG, $2, NULL); }     // ʹ�ø��ŵĹ���
    ;
%%



int main(int argc, char** argv) {
    printf("> ");
     yyparse();
     return 0;
}

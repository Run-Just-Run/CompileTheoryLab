#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "tree.h"

int yyparse(void);

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

void yyerror(char* str) {
    fprintf(stderr, "error:%s\n", str);
}
int main(int argc, char** argv) {
    printf("> ");
    return yyparse();
}

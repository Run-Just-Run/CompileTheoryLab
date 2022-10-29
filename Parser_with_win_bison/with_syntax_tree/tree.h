/* ���������������� */
#pragma once
/* ��ʷ��������Ľӿ� */
extern int yylineno;

void yyerror(char* s);

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

/* ��������﷨�� */
struct ast* newast(int nodetype, struct ast* lft, struct ast* rht);

struct ast* newnum(double d);

/* ��������﷨�� */
double eval(struct ast*);

/* ɾ�����ͷų����﷨�� */
void treefree(struct ast*);

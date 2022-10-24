/* file: 3.02_create_AST_with_bison.y */

%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(char* str) {
    fprintf(stderr, "error:%s\n", str);
}
enum NodeT { // 声明所有语法树节点类型的枚举值
	NT_UNDEF,
	NT_ADD,
	NT_SUB,
	NT_MUL,
	NT_DIV,
	NT_ABS,
	NT_NEG,
	NT_NUM,
};

/* 抽象语法树中的节点 */
struct ast {
	int nodetype;
	struct ast* lft;
	struct ast* rht;
};

struct numval {
	int nodetype; /* 类型K 表明常量 */
	double number;
};

/* 为操作符，创建一个子树 */
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

/* 为一个值，创建一个叶子节点 */
struct ast* newnum(double d) {
    struct numval* a = malloc(sizeof(struct numval));
    if (!a) {
        yyerror("out of memory!");
        exit(0);
    }
    a->nodetype = NT_NUM;
    a->number = d;

    // 此处将"struct numval"的类型强制转换为"struct ast"类型，是因为能够统一适配eval函数的入参类型；
    // 通过nodetype字段识别到"struct numval"类型的实例时，要使用该实例需要强制转换回来才行。
    return (struct ast*)a;
}

/* 递归计算各个子树的值 */
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
    case NT_NEG: // 处理负数求值
        v = eval(a->lft) * -1.0f;
        break;

    default:
        printf("internal error:bad nodetype=%c,%d\n",
            a->nodetype, a->nodetype);
        break;
    }
    return v;
}

/* 递归释放各个树节点 */
void treefree(struct ast* a) {
    switch (a->nodetype) {
    case NT_ADD:/* 2颗子树 */
    case NT_SUB:
    case NT_MUL:
    case NT_DIV:
        treefree(a->lft);
        treefree(a->rht);
        break;
    case NT_ABS: /* 1颗子树 */
    case NT_NEG:
        treefree(a->lft);
        break;
    case NT_NUM:/* 没有子树 */
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

/* %union关键字声明了语法规则中所有语法值可能会用到的数据类型的一个集合 */
%union {
  struct ast *a;
  double d;
}

%token <d> NUMBER /* 声明记号，而"<d>"指定了该记号的数据类型 */
%token EOL

/* 当使用了union声明了一组数据类型集合后，你必须为所有的非终结符指定其值类型。使用"%type"关键字*/
%type <a> exp factor term

%start calclist /* 指定起始符号（start symbol）有时也称为目标符号（goal symbol） */

%%
calclist: /* -- 空规则，起始符号必须具备一个空规则，旨在让起始符号必须匹配整个输入 -- */
        | calclist exp EOL { printf("The expression is legal,the result is:\n");printf("= %4.4g\n", eval($2)); treefree($2); printf("> "); }
        | calclist EOL { printf("> "); } /* 空行或注释 */
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
    | '|' exp '|' { $$ = newast(NT_ABS, $2, NULL); }     // 使用绝对值符号的语法规则
    | '(' exp ')' { $$ = $2; }                           // 使用圆括号的语法规则
    | '-' term    { $$ = newast(NT_NEG, $2, NULL); }     // 使用负号的规则
    ;
%%



int main(int argc, char** argv) {
    printf("> ");
     yyparse();
     return 0;
}

/*********************************************************************************
 * @author: 丁志鹏(OliverDD)
 * @email: oliverdd@std.uestc.edu.cn
 * @desctipiton: 词法分析器。因为仅仅输出语法树，无需具体值，
 *               本词法分析不会制表保留数值，即丢弃无关数值。
 * @Attention: 
 *          1. 调用接口函数void lexicallyAnalyse()，即会更新
 *             外部定义的nToken与nnToken这两个结构体。
 *          2. 调用接口函数前，必须执行
 *             void initLexer(FILE *lex)，用于初始化词法分析器。
 *
*********************************************************************************/


%{

#include "../include/main.h"

int row ,col;

static int lastLeng = 0;

%}


/* 关键字定义 */
VOID                        void
INT                         int
STR                         str
RETURN                      return
MAIN                        main
IF                          if
ELSE                        else
WHILE                       while
PRINT                       print
SCAN                        scan
/* 关键字定义结束 */

/* 运算符定义 */
ASSIGNOP                    [=]
GT                          [>]
GE                          [>][=]
LT                          [<]
LE                          [<][=]
EQ                          [=][=]
NE                          [!][=]
PLUS                        [+]
SUB                         [-]
MUL                         [*]
DIV                         [/]
PERCENT                     [%]
/* 运算符定义结束 */

/* 分界符定义 */
COMMA                       [,]
SEM                         [;]
LP                          [(]
RP                          [)]
LC                          [{]
RC                          [}]
LB                          [[]
RB                          []]
/* 分界符定义结束 */

/* 常数定义 */
STA_NUMBER                  ([1-9][0-9]*)|0
STA_STRING                  \"[^"]*\"

/* 常数定义结束 */

/* 标识符定义 */
ID                          [a-zA-Z_$][a-zA-Z_$0-9]*
/* 标识符定义结束 */

/* 注释定义 */
COMMENT                     \/\/.*
/* 注释定义结束 */

/* Start conditions定义 */

/* Start conditions定义结束 */

%option noyywrap

%%
[ \t]                       lastLeng += 1;
{COMMENT}                   //吃掉
[\n]                        {
                                row++;
                                col = 1;
                                lastLeng = 0;
                            }
{STA_STRING}                {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return STRING_;
                            }
{SCAN}                      {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return SCAN_;
                            }
{VOID}                      {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return VOID_;
                            }
{INT}                       {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return INT_;
                            }
{STR}                       {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return STR_;
                            }
{RETURN}                    {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return RETURN_;
                            }
{MAIN}                      {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return MAIN_;
                            }
{IF}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return IF_;
                            }
{ELSE}                      {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return ELSE_;
                            }
{WHILE}                     {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return WHILE_;
                            }
{PRINT}                     {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return PRINT_;
                            }
{ASSIGNOP}                  {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return ASSIGNOP_;
                            }
{GT}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return GT_;
                            }
{GE}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return GE_;
                            }
{LT}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return LT_;
                            }
{LE}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return LE_;
                            }
{EQ}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return EQ_;
                            }
{NE}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return NE_;
                            }
{PLUS}                      {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return PLUS_;
                            }
{SUB}                       {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return SUB_;
                            }
{MUL}                       {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return MUL_;
                            }
{DIV}                       {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return DIV_;
                            }
{PERCENT}                   {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return PERCENT_;
                            }
{COMMA}                     {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return COMMA_;
                            }
{SEM}                       {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return SEM_;
                            }
{LP}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return LP_;
                            }
{RP}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return RP_;
                            }
{LC}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return LC_;
                            }
{RC}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return RC_;
                            }
{LB}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return LB_;
                            }
{RB}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return RB_;
                            }
{STA_NUMBER}                {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return NUMBER_;
                            }
{ID}                        {
                                col += lastLeng;
                                lastLeng = yyleng;
                                return ID_;
                            }
<<EOF>>                     {
                                return EOF;
                            }
%%
void initLexer(FILE *lex){
    yyin = lex;
    row = 1;
    col = 1;
    nnToken.code = yylex();
    nnToken.row = row;
    nnToken.col = col;
    nnToken.value = (char *)calloc(strlen(yytext), sizeof(char));
	memcpy(nnToken.value, yytext, strlen(yytext));
}
void lexicallyAnalyse(){
    nToken = nnToken;
    if(EOF != nnToken.code){//若nnToken已经抵达文件末尾，则无需读取下一个
        nnToken.code = yylex();
        nnToken.row = row;
        nnToken.col = col;
        nnToken.value = (char *)calloc(strlen(yytext), sizeof(char));
		memcpy(nnToken.value, yytext, strlen(yytext));
    }
#ifdef LOCAL
	printf("当前%s(%d) %d:%d\n", nToken.value, nToken.code, nToken.col, nToken.row);
#endif //LOCAL
}

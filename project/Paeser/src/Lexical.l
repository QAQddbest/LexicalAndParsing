/*********************************************************************************
 * @author: 丁志鹏(OliverDD)
 * @email: oliverdd@std.uestc.edu.cn
 * @desctipiton: 词法分析器。因为仅仅输出语法树，无需具体值，本词法分析不会制表保留数值，即丢弃无关数值。
 * @todo:
 *
*********************************************************************************/


%{
int row ,col;

#define INT_ 1
#define STR_ 2
#define RETURN_ 3
#define MAIN_ 4
#define IF_ 5
#define ELSE_ 6
#define WHILE_ 7
#define PRINT_ 8
#define ASSIGNOP_ 9
#define GT_ 10
#define GE_ 11
#define LT_ 12
#define LE_ 13
#define EQ_ 14
#define NE_ 15
#define PLUS_ 16
#define SUB_ 17
#define MUL_ 18
#define DIV_ 19
#define COMMA_ 20
#define SEM_ 21
#define LP_ 22
#define RP_ 23
#define LC_ 24
#define RC_ 25
#define NUMBER_ 26
#define ID_ 27
#define STRING_ 28

static int lastLeng = 0;

%}


/* 关键字定义 */
INT                         int
STR                         str
RETURN                      return
MAIN                        main
IF                          if
ELSE                        else
WHILE                       while
PRINT                       print
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
/* 运算符定义结束 */

/* 分界符定义 */
COMMA                       [,]
SEM                         [;]
LP                          [(]
RP                          [)]
LC                          [{]
RC                          [}]
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
}
int lexicallyAnalyse(){
    return yylex();
}

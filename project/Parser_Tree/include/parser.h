/*
 * @author: 丁志鹏
 * @date: 2019-11-10
 * @license: MIT
 * @description: The parser.c's head file
 */
#ifndef PARSER_H
#define PARSER_H

#include "tools.h"
#include <stdlib.h>
#include "project.h"
// NULLPATH 赋值给节点的path变量，表示当前节点为暂用节点，不含任何有用信息，仅仅作为桥梁
#define NULLPATH -1
// '+'
#defien ADDPATH -2
// '-'
#define SUBPATH -3
// '*'
#define MULPATH -4
// '/'
#define DIVPATH -5
// '%'
#define PERCENTPATH -6

void initLexer(FILE *);
void lexicallyAnalyse();

static Node *analyseCmpExpr();
static Node *analyseAddExpr();
static Node *analyseMulExpr();
static Node *analysePrimaryExpr();
static Node *analyseExprList();
static Node *analyseIdList();
static Node *analyseProgram();
static Node *analyseExternalDeclaration();
static Node *analyseDeclOrStmt();
static Node *analyseDeclaratorList();
static Node *analyseIntstrList();
static Node *analyseInitializer();
static Node *analyseDeclarator();
static Node *analyseParameterList();
static Node *analyseParameter();
static Node *analyseType();
static Node *analyseStatement();
static Node *analyseStatementList();
static Node *analyseExprStatement();
static Node *analyseExpr();
static Node *analyseCMP();

#endif // PARSER_H

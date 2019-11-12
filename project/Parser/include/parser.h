/*
 * @author: 丁志鹏
 * @date: 2019-11-10
 * @license: MIT
 * @description: The parser.c's head file
 */
#ifndef PARSER_H
#define PARSER_H

#include "tools.h"
#include "project.h"
#include <stdbool.h>

void initLexer(FILE *);
void lexicallyAnalyse();

static bool analyseCmpExpr();
static bool analyseAddExpr();
static bool analyseMulExpr();
static bool analysePrimaryExpr();
static bool analyseExprList();
static bool analyseIdList();
static bool analyseProgram();
static bool analyseExternalDeclaration();
static bool analyseDeclOrStmt();
static bool analyseDeclaratorList();
static bool analyseIntstrList();
static bool analyseInitializer();
static bool analyseDeclarator();
static bool analyseParameterList();
static bool analyseParameter();
static bool analyseType();
static bool analyseStatement();
static bool analyseStatementList();
static bool analyseExprStatement();
static bool analyseExpr();
static bool analyseCMP();

#endif // PARSER_H

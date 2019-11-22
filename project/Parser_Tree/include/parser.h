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
#include <string.h>
#include <stdbool.h>


void initLexer(FILE *);
void lexicallyAnalyse();

bool isEndOfFile();

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

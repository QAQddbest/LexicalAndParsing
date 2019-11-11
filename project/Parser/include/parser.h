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



#endif // PARSER_H

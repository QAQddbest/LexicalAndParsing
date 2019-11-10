/*
 * @author: 丁志鹏
 * @date: 2019-11-01
 * @license: MIT
 * @description: The main.c's head file
 */
#ifndef MAIN_H
#define MAIN_H

#include "project.h"
#include <stdlib.h>
#include <string.h>

#ifdef LOCAL
#include <time.h>
#include "../tools.h"
extern long START_TIME;
#endif // LOCAL

void initLexer(FILE *);
int lexicallyAnalysze(); //词法分析器接口

#endif // MAIN_H

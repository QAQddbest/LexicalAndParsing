/*
 * @author: OliverDD
 * @date: 2019-11-01
 * @license: MIT
 * @description:
 */
#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 定义本地调试模式 */
#define LOCAL
/* 定义本地调试模式 */

#ifdef LOCAL
#include <time.h>
extern long START_TIME;
void logcat(char *);
#endif // LOCAL

void initLexer(FILE *);
int lexicallyAnalysze(); //词法分析器接口

#endif // MAIN_H

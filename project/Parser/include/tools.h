/*
 * @author: 丁志鹏
 * @date: 2019-11-01
 * @license: MIT
 * @description: The main.c's head file
 */
#ifndef TOOLS_H
#define TOOLS_H

#include "project.h"
#ifdef LOCAL
void logcat(char *);
#endif // LOCAL
void throwError(char *, char *);

#endif // TOOLS_H

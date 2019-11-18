#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>

/* 定义本地调试模式 */
#define LOCAL

#define VOID_ 0
#define INT_ 1
#define STR_ 2
#define RETURN_ 3
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
#define LB_ 29
#define RB_ 30
#define SCAN_ 31
#define PERCENT_ 32

#define VIRT_PATH -1
#define TAB_LENG 4

typedef struct{
    int row;            // 行
    int col;            // 列
    int code;           // 种别码
    char *value;        // 本身的值
}Nexttoken;

typedef struct NODE{
    char name[20];      // 节点本身名字
    int path;           // 第几号路径
    char value;         // 本身的值或者路径上的ID
    struct NODE *left;
    struct NODE *right;
    struct NODE *next;  // 同类值的链表指针
}Node;

#ifdef LOCAL

#include <sys/time.h>

struct timeval tv;

#endif // LOCAL

Nexttoken nToken, nnToken;

#endif // PROJECT_H

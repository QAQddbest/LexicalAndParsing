/*
 * @author: 丁志鹏
 * @date: 2019-11-01
 * @license: MIT
 * @description: Some functions that would run anywhere.
 */

#include "../include/tools.h"
#ifdef LOCAL
/* 程序起始时间 */
long START_TIME;

void logcat(char *s) {
    /*
     * @author: 丁志鹏
     * @para: char *
     *        需要用log打印的字符串
     * @return: None
     * @alarm: 使用注意，需在函数运行时确认START_TIME的值
     */
    gettimeofday(&tv, NULL);
    long t = tv.tv_sec * 1000000 + tv.tv_usec;
    printf("%6ld : %s\n", t - START_TIME, s);
}
#endif
void throwError(char *place, char *message) {
    /*
     * @author: 丁志鹏
     * @para:
     * @return: None
     * @alarm:
     */
    printf("致命错误: %s(%d,%d): %s\n", place, nToken.row, nToken.col, message);
}

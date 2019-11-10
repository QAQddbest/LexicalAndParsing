/*
 * @author: 丁志鹏
 * @date: 2019-11-01
 * @license: MIT
 * @description: Some functions that would run anywhere.
 */
long START_TIME;
time_t t;
void logcat(char *s) {
    /*
     * @author: 丁志鹏
     * @para: char *
     *        需要用log打印的字符串
     * @return: None
     * @alarm: 使用注意，需在函数运行时确认START_TIME的值
     */
    t = time(NULL);
    printf("%ld : %s\n", t - START_TIME, s);
}

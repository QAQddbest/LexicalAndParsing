/*
 * @author: OliverDD
 * @date: 2019-11-01
 * @license: MIT
 * @description:
 */
#include <stdio.h>
#include <time.h>
long START_TIME;
time_t t;
void logcat(char *s){
    t = time(NULL);
    printf("%ld : %s\n", t - START_TIME, s);
}

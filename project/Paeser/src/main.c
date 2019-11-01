/*
 * @author: OliverDD
 * @date: 2019-11-01
 * @license: MIT
 * @description:
 */
#include "../include/main.h"

extern int row, col;
extern char *yytext;

int main(int argc, char *argv[]){
    #ifdef LOCAL
    START_TIME = time(NULL);
    logcat("开始运行程序");
    #endif // LOCAL
    FILE *in;
    argc--;
    argv++;
    if(argc == 0){
        printf("LEXER:无输入文件");
        return 1;
    }else{
        while(argc > 0){
            logcat("开始打开文件并运行");
            in = fopen("test1.rd", "r");
            initLexer(in);
            int temp = lexicallyAnalyse();
            while(temp != EOF){
                printf("row = %3d, col = %3d, 种别码 = %3d, 值 = '%s'\n", row, col, temp, yytext);
                temp = lexicallyAnalyse();
            }
            fclose(in);
            argc--;
            argv++;
        }
    }
    return 0;
}

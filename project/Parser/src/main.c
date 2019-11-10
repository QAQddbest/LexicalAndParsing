/*
 * @author: 丁志鹏
 * @date: 2019-11-01
 * @license: MIT
 * @description: 主函数。用户交互、工程入口与总安排者。
 */
#include "../include/main.h"

extern int row, col;
extern char *yytext;

int main(int argc, char *argv[]) {
    /*
     * @author: 丁志鹏
     * @para: int (系统自动传入)
     *        传入参数数量，即待分析的文件数量
     *        char *
     *        传入参数的二维数组，即待分析文件的文件名
     * @return: int
     *          0    程序成功运行结束
     *          1    程序遇到错误并退出
     */
#ifdef LOCAL
    START_TIME = time(NULL);
    logcat("开始运行程序");
#endif // LOCAL
    FILE *in;
    argc--;
    argv++;
    if(argc == 0) {
        printf("LEXER: 无输入文件");
        return 1;
    } else {
        while(argc > 0) {
#ifdef LOCAL
            logcat("尝试打开文件并运行");
#endif
            in = fopen(argv[0], "r");
            if(in == NULL) { // 判断打开文件成功与否
                printf("LEXER: 打开文件%s失败\n", argv[0]);
                argc--;
                argv++;
                continue;
            }
            initLexer(in);
            int temp = lexicallyAnalyse();
            printf("temp = %d\n",temp);
            while(temp != EOF) {
                printf("row = %3d, col = %3d, 种别码 = %3d, 值 = '%s'\n",
                       row, col, temp, yytext);
                temp = lexicallyAnalyse();
            }
            fclose(in);
            argc--;
            argv++;
        }
    }
    return 0;
}

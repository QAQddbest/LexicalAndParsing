/*
 * @author: 丁志鹏
 * @date: 2019-11-01
 * @license: MIT
 * @description: 主函数。用户交互、工程入口与总安排者。
 */
#include "../include/main.h"
#ifdef LOCAL
#include ".././include/tools.h"
extern long START_TIME;
#endif // LOCAL

char TAB_TEMPLATE[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t \
                         \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t \
                         \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t \
                         \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t \
                         \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t \
                         \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t \
                         \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t \
                         \t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

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
    gettimeofday(&tv, NULL);
    START_TIME = tv.tv_sec * 1000000 + tv.tv_usec;
    logcat("开始运行程序");
#endif // LOCAL
    FILE *in;
    Node *root;
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

#ifdef LOCAL
            logcat("开始语法分析");
#endif // LOCAL
            root = parsingAnalyse(in);
            printTheSyntaxTree(root, 0);
#ifdef LOCAL
            logcat("语法分析结束");
#endif // LOCAL
            fclose(in);
            argc--;
            argv++;
        }
    }

    return 0;
}

void printTheSyntaxTree(Node *root, int tabLength){
    if(root->path = NULLPATH){ // 子节点不缩进,本节点不输出
        // 输出左节点
        if(NULL != root->left){
            printTheSyntaxTree(root->left, tabLength);
        }
        // 输出右节点
        if(NULL != root->right){
            printTheSyntaxTree(root->right, tabLength);
        }
        // 输出next节点
        if(NULL != root->next){
            printTheSyntaxTree(root->next, tabLength);
        }
    }else{
        char *tab = (char *)calloc(tabLength+1, sizeof(char));
        memcpy(tab, TAB_TEMPLATE, tabLength);
        tab[tabLength] = '\0';
        // 输出本节点
        printf("%s%5s:", root->name);
        if(NULL != root->value){
            printf("%5s:", root->name);
        }else{
            printf("%5s:", ' ');
        }
        printf("%5d", root->path);
        // 输出左节点
        if(NULL != root->left){
            printTheSyntaxTree(root->left, tabLength + TAB_LENG);
        }
        // 输出右节点
        if(NULL != root->right){
            printTheSyntaxTree(root->right, tabLength + TAB_LENG);
        }
        // 输出next节点
        if(NULL != root->next){
            printTheSyntaxTree(root->next, tabLength);
        }
    }
}

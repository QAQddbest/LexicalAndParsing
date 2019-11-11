/*
 * @author: 丁志鹏
 * @date: 2019-11-10
 * @license: MIT
 * @description: 词法分析所需的全部主函数。每读取一个终结符，调用一次lexicallyAnalyse()
 */
#include "../include/parser.h"
#ifdef LOCAL
#include "tools.h"
#endif // LOCAL

static bool analyseCmpExpr(){
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    if(true != analyseExpr()){
        throwError("cmp_expr", "分析Expr时出错");
        return false;
    }
    while(true == analyseExpr()){}//循环判断
    return true;
}

static bool analyseAddExpr(){
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    if(true != analyseMulExpr()){
        throwError("add_expr", "分析mul_expr时出错");
        return false;
    }
    while(SUB_ == nToken.code || PLUS_ == nToken.code){
        lexicallyAnalyse();
        if(true != analyseMulExpr()){
            throwError("add_expr", "分析mul_expr时出错");
            return false;
        }
    }
    return true;
}

static bool analyseMulExpr(){
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    while(SUB_ == nToken.code){//吃掉多余负号
        lexicallyAnalyse();
    }
    if(true != analysePrimaryExpr()){
        throwError("add_expr", "分析primary_expr时出错");
        return false;
    }
    while(MUL_ == nToken.code || DIV_ == nToken.code || PERCENT_ == nToken.code){
        lexicallyAnalyse();
        if(true != analysePrimaryExpr()){
            throwError("add_expr", "分析primary_expr时出错");
            return false;
        }
    }
    return true;
}

static bool analysePrimaryExpr(){
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    if(NUMBER_ == nToken.code || STRING_ == nToken.code){//判断数字与字符串
        lexicallyAnalyse();
    }else if(LP_ == nToken.code){//判断到左括号
        lexicallyAnalyse();
        if(true != analyseExpr()){
            throwError("primary_expr", "格式'( expr )'错误");
            return false;
        }
        if(RP_ == nToken.code)
            lexicallyAnalyse();
        else
            return false;
    }else if(ID_ == nToken.code){//判断到ID
        lexicallyAnalyse();
        if(LP_ == nToken.code){///检测到左括号
             lexicallyAnalyse();
             if(RB_ == nToken.code){////若为ID '(' ')'，直接返回true
                lexicallyAnalyse();
                return true;
             }
             if(true != analyseExprList()){////再判断expr_list
                 throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                 return false;
             }
        }else if(ASSIGNOP_ == nToken.code){///检测到等于号
             lexicallyAnalyse();
            if(true != analyseExprList()){////再判断expr_list
                 throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                 return false;
             }
        }else if(LB_ == nToken.code){///检测到左中括号
            lexicallyAnalyse();
            if(true != analyseExpr()){////再判断expr
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return false;
            }
            if(RB_ != nToken.code){////再判断右中括号
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return false;
            }
            lexicallyAnalyse();////已经匹配了右中括号,读入下一个
            if(ASSIGNOP_ == nToken.code){/* ！发现异常！ */
                /* 若发现=，但后不跟expr，就会报错。但也有可能=是其他的 */
                lexicallyAnalyse();
                if(true != analyseExpr()){
                    throwError("primary_expr", "格式ID '[' expr ']' '=' expr错误");
                    return false;
                }
            }
        }///判断到ID的内部if
    }else{//没有匹配项目
        throwError("primary_expr", "分析primay_expr时无匹配格式");
        return false;
    }//结束
    return true;
}










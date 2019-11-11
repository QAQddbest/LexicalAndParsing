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
    }else{
        while(true == analyseExpr()){//循环判断

        }
    }
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
    }else{
        while(SUB_ == nToken.code || PLUS_ == nToken.code){
            lexicallyAnalyse();
            if(true != analyseMulExpr()){
                throwError("add_expr", "分析mul_expr时出错");
                return false;
            }
        }
    }
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
    }else{
        while(MUL_ == nToken.code || DIV_ == nToken.code
                                  || PERCENT_ == nToken.code){
            lexicallyAnalyse();
            if(true != analysePrimaryExpr()){
                throwError("add_expr", "分析primary_expr时出错");
                return false;
            }
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

}










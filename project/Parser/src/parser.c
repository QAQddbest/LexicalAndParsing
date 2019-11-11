/*
 * @author: 丁志鹏
 * @date: 2019-11-10
 * @license: MIT
 * @description: 词法分析所需的全部主函数。每读取一个终结符，调用一次lexicallyAnalyse()
 */
#include "../include/parser.h"
#ifdef LOCAL
#include "../include/tools.h"
#endif // LOCAL


void parsingAnalyse(FILE *in) {
    initLexer(in);
    lexicallyAnalyse();
    if(false == analyseProgram()){
        throwError("语法分析","语法分析时出错");
    }else{
        printf("执行完毕");
    }
}

static bool analyseProgram() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别Program
    *          false   不符合Program
    */
    if(analyseExternalDeclaration() == true) {
        while(nToken.code != EOF) {
            if(analyseExternalDeclaration() == false) {
                throwError("program","分析external_declaration时出错");
                return false;
            }
        }
        return true;
    } else {
        throwError("program","分析external_declaration时出错");
        return false;
    }
}

static bool analyseExternalDeclaration() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: bool
     *          true    成功识别ExternalDeclaration
     *          false   不符合ExternalDeclaration
     */
    if(analyseType() == true) {
#ifdef LOCAL
        printf("external_declaration:%d:%d 正在分析%s ( %d )\n",nToken.col, nToken.row, nToken.value, nToken.code);
#endif // LOCAL
        if(analyseDeclarator() == true) {
            if(analyseDeclOrStmt() == true) {
                return true;
            } else {
                throwError("external_declaration","分析decl_or_stmt时出错");
                return false;
            }
        } else {
            throwError("external_declaration","分析declarator时出错");
            return false;
        }
    } else {
        throwError("external_declaration","分析type时出错");
        return false;
    }
}

static bool analyseDeclOrStmt() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别DeclOrStmt
    *          false   不符合DeclOrStmt
    */
    if(nToken.code == LC_) { // '{'
        lexicallyAnalyse();
        if(nToken.code == RC_) { // '}'
            lexicallyAnalyse();
            return true;
        } else if(analyseStatementList() == true) {
            if(nToken.code == RC_) { // '}'
                lexicallyAnalyse();
                return true;
            } else {
                throwError("decl_or_stmt","分析'}'时出错");
                return false;
            }
        } else {
            throwError("decl_or_stmt","结构错误");
            return false;
        }
    } else if(nToken.code == COMMA_) { //','
        lexicallyAnalyse();
        if(analyseDeclaratorList() == true) {
            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                return true;
            } else {
                throwError("decl_or_stmt","分析';'时出错");
                return false;
            }
        } else {
            throwError("decl_or_stmt","分析declarator_list时出错");
            return false;
        }
    } else if(nToken.code == SEM_) { // ';'
        lexicallyAnalyse();
        return true;
    } else {
        throwError("decl_or_stmt","结构错误");
        return false;
    }
}

static bool analyseDeclaratorList() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别DeclaratorList
    *          false   不符合DeclaratorList
    */
    if(analyseDeclarator() == true) {
        while(nToken.code == COMMA_) { //','
            if(analyseDeclarator() == false) {
                throwError("declarator_list","分析declarator时出错");
                return false;
            }
        }
        return true;
    } else {
        throwError("declarator_list","分析declarator时出错");
        return false;
    }
}

static bool analyseIntstrList() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别InstrList
    *          false   不符合InstrList
    */
    if(analyseInitializer() == true) {
        while(nToken.code == COMMA_) { //','
            if(analyseInitializer() == false) {
                throwError("declarator_list","分析initializer时出错");
                return false;
            }
        }
        return true;
    } else {
        throwError("declarator_list","分析initializer时出错");
        return false;
    }
}

static bool analyseInitializer() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别Initializer
    *          false   不符合Initializer
    */
    if(nToken.code == NUMBER_ || nToken.code == STR_) {
        lexicallyAnalyse();
        return true;
    } else {
        throwError("initializer","分析NUMBER或STRING时出错");
        return false;
    }
}

static bool analyseDeclarator() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别Declarator
    *          false   不符合Declarator
    */
#ifdef LOCAL
    printf("declarator:%d:%d 正在分析%s ( %d )\n",nToken.col, nToken.row, nToken.value, nToken.code);
#endif // LOCAL
    if(nToken.code == ID_) {
        lexicallyAnalyse();
        if(nToken.code == ASSIGNOP_) { // '='
            if(analyseExpr() == true) {
                return true;
            } else {
                throwError("declarator","分析expr时出错");
                return false;
            }
        } else if(nToken.code == LP_) { // '('
            lexicallyAnalyse();
            if(nToken.code == RP_) { // ')'
                lexicallyAnalyse();
                return true;
            } else if(analyseParameterList() == true) {
                if(nToken.code == RP_) { // ')'
                    lexicallyAnalyse();
                    return true;
                } else {
                    throwError("declarator","分析')'时出错");
                    return false;
                }
            } else {
                throwError("declarator","分析')'或parameter_list时出错");
                return false;
            }
        } else if(nToken.code == LB_) { // '['
            lexicallyAnalyse();
            if(nToken.code == RB_) { // ']'
                lexicallyAnalyse();
                if(nToken.code == ASSIGNOP_) { // '='
                    lexicallyAnalyse();
                    if(nToken.code == LC_) { // '{'
                        lexicallyAnalyse();
                        if(analyseIntstrList() == true) {
                            if(nToken.code == RC_) { // '}'
                                lexicallyAnalyse();
                                return true;
                            } else {
                                throwError("declarator","分析'}'时出错");
                                return false;
                            }
                        } else {
                            throwError("declarator","分析intstr_list时出错");
                            return false;
                        }
                    } else {
                        throwError("declarator","分析'{'时出错");
                        return false;
                    }
                } else {
                    return true; // ']' 后面允许不跟任何东西
                }
            } else if(analyseExpr() == true) {
                if(nToken.code == RB_) { // ']'
                    lexicallyAnalyse();
                    if(nToken.code == ASSIGNOP_) { // '='
                        lexicallyAnalyse();
                        if(nToken.code == LC_) { // '{'
                            lexicallyAnalyse();
                            if(analyseIntstrList() == true) {
                                if(nToken.code == RC_) { // '}'
                                    lexicallyAnalyse();
                                    return true;
                                } else {
                                    throwError("declarator","分析'}'时出错");
                                    return false;
                                }
                            } else {
                                throwError("declarator","分析intstr_list时出错");
                                return false;
                            }
                        } else {
                            throwError("declarator","分析'{'时出错");
                            return false;
                        }
                    } else {
                        return true; // ']' 后面允许不跟任何东西
                    }
                } else {
                    throwError("declarator","分析']'时出错");
                    return false;
                }
            } else {
                throwError("declarator","结构错误");
                return false;
            }
        } else {
            return true; // ID 后面允许不跟任何东西
        }
    } else {
#ifdef LOCAL
        printf("declarator:%d:%d 正在分析%s ( %d )\n",nToken.col, nToken.row, nToken.value, nToken.code);
#endif // LOCAL
        throwError("declarator","分析ID时出错");
        return false;
    }
}

static bool analyseParameterList() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别ParameterList
    *          false   不符合ParameterList
    */
    if(analyseParameter() == true) {
        while(nToken.code == COMMA_) { // ',' COMMA_
            lexicallyAnalyse();
            if(analyseParameter() == false) {
                throwError("parameter_list","分析','时出错");
                return false;
            }
        }
        return true;
    } else {
        throwError("parameter_list","结构错误");
        return false;
    }
}

static bool analyseParameter() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别Parameter
    *          false   不符合Parameter
    */
    if(analyseType() == true) {
        lexicallyAnalyse();
        if(nToken.code == ID_) {
            lexicallyAnalyse();
            return true;
        } else {
            // error();
            throwError("parameter","分析ID时出错");
            return false;
        }
    } else {
        throwError("parameter","结构错误");
        return false;
    }
}

static bool analyseType() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别Type
    *          false   不符合Type
    */
    if(nToken.code == INT_ || nToken.code == STR_ || nToken.code == VOID_) {
        lexicallyAnalyse();
        return true;
    } else {
        // error()
        throwError("type","结构错误");
        return false;
    }
}

static bool analyseStatement() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别Statement
    *          false   不符合Statement
    */
    if(nToken.code == LC_) { // '{' LC
        lexicallyAnalyse();
        if(analyseStatementList() == true) {
            if(nToken.code == RC_) { // '}' RC
                lexicallyAnalyse();
                return true;
            } else {
                // error()
                throwError("statement","分析'}'时出错");
                return false;
            }
        } else {
            throwError("statement","分析statement时出错");
            return false;
        }
    } else if(nToken.code == IF_) {
        lexicallyAnalyse();
        if(nToken.code == LP_) { // '(' LP
            lexicallyAnalyse();
            if(analyseExpr() == true) {
                if(nToken.code == RP_) { // ')' RB
                    lexicallyAnalyse();
                    if(analyseStatement() == true) {
                        if(nToken.code == ELSE_) {
                            lexicallyAnalyse();
                            if(analyseStatement() == true) {
                                return true;
                            } else {
                                throwError("statement","分析statement时出错");
                                return false;
                            }
                        } else {
                            throwError("statement","分析ELSE时出错");
                            return true; // statement 后面允许不接其他东西
                        }
                    } else {
                        throwError("statement","分析statement时出错");
                        return false;
                    }
                } else {
                    // error()
                    throwError("statement","分析')'时出错");
                    return false;
                }
            } else {
                throwError("statement","分析expr时出错");
                return false;
            }
        } else {
            // error()
            throwError("statement","分析'('时出错");
            return false;
        }
    } else if(nToken.code == WHILE_) {
        lexicallyAnalyse();
        if(nToken.code == LP_) { // '('
            lexicallyAnalyse();
            if(analyseExpr() == true) {
                if(nToken.code == RP_) { // ')'
                    lexicallyAnalyse();
                    if(analyseStatement() == true) {
                        return true;
                    } else {
                        throwError("statement","分析statement时出错");
                        return false;
                    }
                } else {
                    // error()
                    throwError("statement","分析')'时出错");
                    return false;
                }
            } else {
                throwError("statement","分析expr时出错");
                return false;
            }
        } else {
            // error()
            throwError("statement","分析'('时出错");
            return false;
        }
    } else if(nToken.code == RETURN_) {
        lexicallyAnalyse();
        if(nToken.code == SEM_) { // ';'
            lexicallyAnalyse();
            return true;
        } else if(analyseExpr() == true) {
            if(nToken.code == SEM_) { // ';'
                lexicallyAnalyse();
                return true;
            } else {
                // error()
                throwError("statement","分析';'时出错");
                return false;
            }
        } else {
            throwError("statement","分析expr时出错");
            return false;
        }
    } else if(nToken.code == PRINT_) {
        lexicallyAnalyse();
        if(nToken.code == SEM_) { //';'
            lexicallyAnalyse();
            return true;
        } else if(analyseExprList() == true) {
            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                return true;
            } else {
                //    error()
                throwError("statement","分析';'时出错");
                return false;
            }
        } else {
            throwError("statement","分析expr_list时出错");
            return false;
        }
    } else if(nToken.code == SCAN_) {
        lexicallyAnalyse();
        if(analyseIdList() == true) {
            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                return true;
            } else {
                // error()
                throwError("statement","分析';'时出错");
                return false;
            }
        } else {
            throwError("statement","分析id_list时出错");
            return false;
        }
    } else if(analyseType() == true) {
        if(analyseDeclaratorList() == true) {
            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                return true;
            } else {
                // error()
                throwError("statement","分析';'时出错");
                return false;
            }
        } else {
            throwError("statement","分析declarator_list时出错");
            return false;
        }
    } else if(analyseExprStatement() == true) {
        return true;
    } else {
        throwError("statement","结构错误");
        return false;
    }
}

static bool analyseStatementList() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: bool
     *          true    成功识别IntstrList
     *          false   不符合IntstrList
     */
    if(analyseStatement() == true) {
        while(nToken.code != RC_) { //'}'
            lexicallyAnalyse();
            if(analyseStatement() != true) {
                throwError("statement_list", "分析statement时出错");
                return false;
            }
        }
        return true;
    } else {
        throwError("statement_list", "结构错误");
        return false;
    }
}

static bool analyseExprStatement() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: bool
     *          true    成功识别ExprStatement
     *          false   不符合ExprStatement
     */
    if(nToken.code == SEM_) { //';'
        lexicallyAnalyse();
        return true;
    } else if(analyseExpr() == true) {
        if(nToken.code == SEM_) { //';'
            lexicallyAnalyse();
            return true;
        } else {
            // error()
            throwError("expression_statement", "分析';'时出错");
            return false;
        }
    } else {
        throwError("expression_statement", "结构错误");
        return false;
    }
}

static bool analyseExpr() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: bool
     *          true    成功识别Expr
     *          false   不符合Expr
     */
    if(analyseCmpExpr() == true) {
        return true;
    } else {
        throwError("expr", "结构错误");
        return false;
    }
}

static bool analyseCmpExpr() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    if(true != analyseExpr()) {
        throwError("cmp_expr", "分析Expr时出错");
        return false;
    }
    while(true == analyseExpr()) {} //循环判断
    return true;
}

static bool analyseAddExpr() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    if(true != analyseMulExpr()) {
        throwError("add_expr", "分析mul_expr时出错");
        return false;
    }
    while(SUB_ == nToken.code || PLUS_ == nToken.code) {
        lexicallyAnalyse();
        if(true != analyseMulExpr()) {
            throwError("add_expr", "分析mul_expr时出错");
            return false;
        }
    }
    return true;
}

static bool analyseMulExpr() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    while(SUB_ == nToken.code) { //吃掉多余负号
        lexicallyAnalyse();
    }
    if(true != analysePrimaryExpr()) {
        throwError("add_expr", "分析primary_expr时出错");
        return false;
    }
    while(MUL_ == nToken.code || DIV_ == nToken.code || PERCENT_ == nToken.code) {
        lexicallyAnalyse();
        if(true != analysePrimaryExpr()) {
            throwError("add_expr", "分析primary_expr时出错");
            return false;
        }
    }
    return true;
}

static bool analysePrimaryExpr() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    if(NUMBER_ == nToken.code || STRING_ == nToken.code) { //判断数字与字符串
        lexicallyAnalyse();
    } else if(LP_ == nToken.code) { //判断到左括号
        lexicallyAnalyse();
        if(true != analyseExpr()) {
            throwError("primary_expr", "格式'( expr )'错误");
            return false;
        }
        if(RP_ == nToken.code)
            lexicallyAnalyse();
        else
            return false;
    } else if(ID_ == nToken.code) { //判断到ID
        lexicallyAnalyse();
        if(LP_ == nToken.code) { ///检测到左括号
            lexicallyAnalyse();
            if(RB_ == nToken.code) { ////若为ID '(' ')'，直接返回true
                lexicallyAnalyse();
                return true;
            }
            if(true != analyseExprList()) { ////再判断expr_list
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return false;
            }
        } else if(ASSIGNOP_ == nToken.code) { ///检测到等于号
            lexicallyAnalyse();
            if(true != analyseExprList()) { ////再判断expr_list
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return false;
            }
        } else if(LB_ == nToken.code) { ///检测到左中括号
            lexicallyAnalyse();
            if(true != analyseExpr()) { ////再判断expr
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return false;
            }
            if(RB_ != nToken.code) { ////再判断右中括号
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return false;
            }
            lexicallyAnalyse();////已经匹配了右中括号,读入下一个
            if(ASSIGNOP_ == nToken.code) { /* ！发现异常！ */
                /* 若发现=，但后不跟expr，就会报错。但也有可能=是其他的 */
                lexicallyAnalyse();
                if(true != analyseExpr()) {
                    throwError("primary_expr", "格式ID '[' expr ']' '=' expr错误");
                    return false;
                }
            }
        }///判断到ID的内部if
    } else { //没有匹配项目
        throwError("primary_expr", "分析primay_expr时无匹配格式");
        return false;
    }//结束
    return true;
}


static bool analyseIdList() {
    /*
    * @author: 王明远
    * @para: None
    * @return: bool
    *          true    成功识别IdList
    *          false   不符合IdList
    */
    if(nToken.code == ID_ ) {
        lexicallyAnalyse();
        while(nToken.code == COMMA_) { // ',' COMMA_
            lexicallyAnalyse();
            if(nToken.code != ID_) {
                throwError("id_list", "分析ID时出错");
                return false;
            }
        }
        return true;
    } else {
        throwError("id_list", "无匹配出错");
        return false;
    }
}

static bool analyseExprList() {
    /*
    * @author: 王明远
    * @para: None
    * @return: bool
    *          true    成功识别ExprList
    *          false   不符合ExprList
    */
    if(analyseExpr() == true) {
        lexicallyAnalyse();
        while(nToken.code == COMMA_) { // ',' COMMA_
            lexicallyAnalyse();
            if(analyseExpr() == false) {
                throwError("expr_list","分析expr时出错");
                return false;
            }
        }
        return true;
    } else {
        throwError("id_list", "无匹配出错");
        return false;
    }
}






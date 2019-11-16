/*
 * @author: 丁志鹏
 * @date: 2019-11-10
 * @license: MIT
 * @description: 词法分析所需的全部主函数。每读取一个终结符，调用一次lexicallyAnalyse()
 *               next变量存储了同级同类的节点
 *               统一存入左子树，若剩余节点不为1,则建立空节点，再接上。便于后续输出
 */
#include "../include/parser.h"
#ifdef LOCAL
#include "../include/tools.h"
#endif // LOCAL


Node *parsingAnalyse(FILE *in) {
    /*
    * @author: 丁志鹏
    * @para: FILE *
    * @return: bool
    *          Node *    成功识别Program,并返回该节点内容
    *          NULL      识别失败
    */
    // 按照规定初始化词法分析器
    initLexer(in);
    // 预载入一个值
    lexicallyAnalyse();
    // 开始语法分析
    Node *root = analyseProgram();
    if(NULL == root)
        throwError("语法分析","语法分析时出错");
    return root;
}

static Node *analyseProgram() {
    /*
    * @author: 丁志鹏
    * @para: None
    * @return: bool
    *          Node *    成功识别Program
    *          NULL      不符合Program
    */
    // 声明变量
    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    // 初始化变量
    memcpy(root->name, "program", sizeof("program"));
    next = analyseExternalDeclaration();
    temp = next;
    if(NULL != next) {                              // 判断了至少一个external_declaration
        root->left = next;                          // 加入program节点的左节点
        next = analyseExternalDeclaration();        // 再次判断是否存在
        while(NULL != left){                        // 若仍成功
            temp->next = next;                      // 将该同类节点放入next
            temp = temp->next;                      // temp前移
            left = analyseExternalDeclaration();    // 再次判断是否存在
        }
    }else{                                          // 一个external_declaration都没有：抛出错误
        throwError("program","分析external_declaration时出错");
    }
    return root;
}

static Node *analyseExternalDeclaration() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: bool
     *          true    成功识别ExternalDeclaration
     *          false   不符合ExternalDeclaration
     */
    if(analyseType() == true) {
#ifdef LOCALL
        printf("external_declaration:%d:%d 正在分析%s ( %d )\n",nToken.row, nToken.col, nToken.value, nToken.code);
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

static Node *analyseDeclOrStmt() {
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

static Node *analyseDeclaratorList() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别DeclaratorList
    *          false   不符合DeclaratorList
    */
    if(analyseDeclarator() == true) {
        while(nToken.code == COMMA_) { //','
            lexicallyAnalyse();
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

static Node *analyseIntstrList() {
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

static Node *analyseInitializer() {
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

static Node *analyseDeclarator(){
    if(ID_ != nToken.code){
        throwError("declarator","匹配ID失败");
        return false;
    }
    lexicallyAnalyse();
    if(ASSIGNOP_ == nToken.code){// =
        lexicallyAnalyse();
        if(true != analyseExpr()){
            throwError("declarator","匹配expr时失败");
            return false;
        }
    }else if(LP_ == nToken.code){// (
        lexicallyAnalyse();
        if(RP_ == nToken.code){// 若直接发现()，直接返回true
            lexicallyAnalyse();
            return true;
        }
        if(true != analyseParameterList()){
            throwError("declarator","匹配parameter_list失败");
            return false;
        }
        if(RP_ != nToken.code) {
            throwError("declarator", "格式ID '(' expr_list ')'丢失右括号");
            return false;
        }
        lexicallyAnalyse();
    }else if(LC_ == nToken.code){// {
        lexicallyAnalyse();
        if(RC_ == nToken.code){// 若直接发现[]，直接返回true
            lexicallyAnalyse();
            return true;
        }
        if(true != analyseExpr()){
            throwError("declarator","匹配expr失败");
            return false;
        }
        if(RC_ != nToken.code) {
            throwError("declarator", "格式ID '[' expr ']'丢失右括号");
            return false;
        }
        lexicallyAnalyse();
    }else if(LB_ == nToken.code){// [
        lexicallyAnalyse();
        if(RB_ == nToken.code){
            lexicallyAnalyse();
        }else if( true == analyseExpr()){
            if(RB_ != nToken.code){
                throwError("declarator", "右中括号不匹配");
                return false;
            }
            lexicallyAnalyse();
        }else{
            throwError("declarator", "判断时无匹配选项");
            return false;
        }
        if(ASSIGNOP_ != nToken.code){
            throwError("declarator", "等号不匹配");
            return false;
        }
        lexicallyAnalyse();
        if(LC_ != nToken.code){
            throwError("declarator", "左大括号不匹配");
            return false;
        }
        lexicallyAnalyse();
        if(true != analyseIntstrList()){
            throwError("declarator", "分析intstr_list时出错");
            return false;
        }
        if(RC_ != nToken.code){
            throwError("declarator", "右大括号不匹配");
            return false;
        }
        lexicallyAnalyse();
    }
    return true;
}




static Node *analyseParameterList() {
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

static Node *analyseParameter() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          true    成功识别Parameter
    *          false   不符合Parameter
    */
    if(analyseType() == true) {
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

static Node *analyseType() {
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
    } else if(ID_ == nToken.code){
        return true;
    }else{
        // error()
        throwError("type","结构错误");
        return false;
    }
}

static Node *analyseStatement() {
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

static Node *analyseStatementList() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: bool
     *          true    成功识别IntstrList
     *          false   不符合IntstrList
     */
    if(analyseStatement() == true) {
        while(nToken.code != RC_) { //'}'
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

static Node *analyseExprStatement() {
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

static Node *analyseExpr() {
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

static Node *analyseCmpExpr() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          true    成功识别
     *          false   不符合
     */
    if(true != analyseAddExpr()) {
        throwError("cmp_expr", "分析Expr时出错");
        return false;
    }
    while(true == analyseCMP()) {
        if(true != analyseAddExpr()){
            throwError("cmp_expr", "无法分析add_expr");
            return false;
        }
    }
    return true;
}

static Node *analyseCMP(){
    if(LE_ == nToken.code){
        lexicallyAnalyse();
    }else if(GE_ == nToken.code){
        lexicallyAnalyse();
    }else if(EQ_ == nToken.code){
        lexicallyAnalyse();
    }else if(GT_ == nToken.code){
        lexicallyAnalyse();
    }else if(LT_ == nToken.code){
        lexicallyAnalyse();
    }else{
        return false;
    }
    return true;
}

static Node *analyseAddExpr() {
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

static Node *analyseMulExpr() {
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

static Node *analysePrimaryExpr() {
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
            if(RP_ == nToken.code) { ////若为ID '(' ')'，直接返回true
                lexicallyAnalyse();
                return true;
            }
            if(true != analyseExprList()) { ////再判断expr_list
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return false;
            }
            if(RP_ != nToken.code) { ///检测到右括号
                throwError("primary_expr", "格式ID '(' expr_list ')'丢失右括号");
                return false;
            }
            lexicallyAnalyse();
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


static Node *analyseIdList() {
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

static Node *analyseExprList() {
    /*
    * @author: 王明远
    * @para: None
    * @return: bool
    *          true    成功识别ExprList
    *          false   不符合ExprList
    */
    if(analyseExpr() == true) {
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






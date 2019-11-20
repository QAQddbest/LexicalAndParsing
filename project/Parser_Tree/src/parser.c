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
        throwError("语法分析", "语法分析时出错");

    return root;
}
/* 样例函数：仿照本函数实现剩余函数 */
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

        while(NULL != next) {                       // 若仍成功
            temp->next = next;                      // 将该同类节点放入next
            temp = temp->next;                      // temp后移
            next = analyseExternalDeclaration();    // 再次判断是否存在
        }
    } else {                                         // 一个external_declaration都没有：抛出错误
        throwError("program", "分析external_declaration时出错");
        return NULL;
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
        printf("external_declaration:%d:%d 正在分析%s ( %d )\n", nToken.row, nToken.col, nToken.value, nToken.code);
#endif // LOCAL

        if(analyseDeclarator() == true) {
            if(analyseDeclOrStmt() == true) {
                return true;
            } else {
                throwError("external_declaration", "分析decl_or_stmt时出错");
                return false;
            }
        } else {
            throwError("external_declaration", "分析declarator时出错");
            return false;
        }
    } else {
        throwError("external_declaration", "分析type时出错");
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
                throwError("decl_or_stmt", "分析'}'时出错");
                return false;
            }
        } else {
            throwError("decl_or_stmt", "结构错误");
            return false;
        }
    } else if(nToken.code == COMMA_) { //','
        lexicallyAnalyse();

        if(analyseDeclaratorList() == true) {
            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                return true;
            } else {
                throwError("decl_or_stmt", "分析';'时出错");
                return false;
            }
        } else {
            throwError("decl_or_stmt", "分析declarator_list时出错");
            return false;
        }
    } else if(nToken.code == SEM_) { // ';'
        lexicallyAnalyse();
        return true;
    } else {
        throwError("decl_or_stmt", "结构错误");
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
                throwError("declarator_list", "分析declarator时出错");
                return false;
            }
        }

        return true;
    } else {
        throwError("declarator_list", "分析declarator时出错");
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
                throwError("declarator_list", "分析initializer时出错");
                return false;
            }
        }

        return true;
    } else {
        throwError("declarator_list", "分析initializer时出错");
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
        throwError("initializer", "分析NUMBER或STRING时出错");
        return false;
    }
}

static Node *analyseDeclarator() {
    if(ID_ != nToken.code) {
        throwError("declarator", "匹配ID失败");
        return false;
    }

    lexicallyAnalyse();

    if(ASSIGNOP_ == nToken.code) { // =
        lexicallyAnalyse();

        if(true != analyseExpr()) {
            throwError("declarator", "匹配expr时失败");
            return false;
        }
    } else if(LP_ == nToken.code) { // (
        lexicallyAnalyse();

        if(RP_ == nToken.code) { // 若直接发现()，直接返回true
            lexicallyAnalyse();
            return true;
        }

        if(true != analyseParameterList()) {
            throwError("declarator", "匹配parameter_list失败");
            return false;
        }

        if(RP_ != nToken.code) {
            throwError("declarator", "格式ID '(' expr_list ')'丢失右括号");
            return false;
        }

        lexicallyAnalyse();
    } else if(LC_ == nToken.code) { // {
        lexicallyAnalyse();

        if(RC_ == nToken.code) { // 若直接发现[]，直接返回true
            lexicallyAnalyse();
            return true;
        }

        if(true != analyseExpr()) {
            throwError("declarator", "匹配expr失败");
            return false;
        }

        if(RC_ != nToken.code) {
            throwError("declarator", "格式ID '[' expr ']'丢失右括号");
            return false;
        }

        lexicallyAnalyse();
    } else if(LB_ == nToken.code) { // [
        lexicallyAnalyse();

        if(RB_ == nToken.code) {
            lexicallyAnalyse();
        } else if( true == analyseExpr()) {
            if(RB_ != nToken.code) {
                throwError("declarator", "右中括号不匹配");
                return false;
            }

            lexicallyAnalyse();
        } else {
            throwError("declarator", "判断时无匹配选项");
            return false;
        }

        if(ASSIGNOP_ != nToken.code) {
            throwError("declarator", "等号不匹配");
            return false;
        }

        lexicallyAnalyse();

        if(LC_ != nToken.code) {
            throwError("declarator", "左大括号不匹配");
            return false;
        }

        lexicallyAnalyse();

        if(true != analyseIntstrList()) {
            throwError("declarator", "分析intstr_list时出错");
            return false;
        }

        if(RC_ != nToken.code) {
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
                throwError("parameter_list", "分析','时出错");
                return false;
            }
        }

        return true;
    } else {
        throwError("parameter_list", "结构错误");
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
            throwError("parameter", "分析ID时出错");
            return false;
        }
    } else {
        throwError("parameter", "结构错误");
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
    } else if(ID_ == nToken.code) {
        return true;
    } else {
        // error()
        throwError("type", "结构错误");
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
                throwError("statement", "分析'}'时出错");
                return false;
            }
        } else {
            throwError("statement", "分析statement时出错");
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
                                throwError("statement", "分析statement时出错");
                                return false;
                            }
                        } else {
                            throwError("statement", "分析ELSE时出错");
                            return true; // statement 后面允许不接其他东西
                        }
                    } else {
                        throwError("statement", "分析statement时出错");
                        return false;
                    }
                } else {
                    // error()
                    throwError("statement", "分析')'时出错");
                    return false;
                }
            } else {
                throwError("statement", "分析expr时出错");
                return false;
            }
        } else {
            // error()
            throwError("statement", "分析'('时出错");
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
                        throwError("statement", "分析statement时出错");
                        return false;
                    }
                } else {
                    // error()
                    throwError("statement", "分析')'时出错");
                    return false;
                }
            } else {
                throwError("statement", "分析expr时出错");
                return false;
            }
        } else {
            // error()
            throwError("statement", "分析'('时出错");
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
                throwError("statement", "分析';'时出错");
                return false;
            }
        } else {
            throwError("statement", "分析expr时出错");
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
                throwError("statement", "分析';'时出错");
                return false;
            }
        } else {
            throwError("statement", "分析expr_list时出错");
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
                throwError("statement", "分析';'时出错");
                return false;
            }
        } else {
            throwError("statement", "分析id_list时出错");
            return false;
        }
    } else if(analyseType() == true) {
        if(analyseDeclaratorList() == true) {
            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                return true;
            } else {
                // error()
                throwError("statement", "分析';'时出错");
                return false;
            }
        } else {
            throwError("statement", "分析declarator_list时出错");
            return false;
        }
    } else if(analyseExprStatement() == true) {
        return true;
    } else {
        throwError("statement", "结构错误");
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
     *          Node *  成功识别并返回节点
     *          NULL    识别失败
     */
    // 声明变量
    Node *next, *temp, *left, *right;
    Node *root = (Node *)calloc(1, sizeof(Node));
    // 初始化变量
    memcpy(root->name, "cmp_expr", sizeof("cmp_expr"));
    left = analyseAddExpr();

    if(NULL == left) {
        throwError("cmp_expr", "分析Expr时出错");
        return NULL;
    }

    temp = root;
    root->left = left;
    right = analyseCMP();

    while(NULL != right) {
        temp->right = right;
        temp = temp->right;
        left = analyseAddExpr();

        if(NULL == left) {
            throwError("cmp_expr", "无法分析add_expr");
            return NULL;
        }

        temp->left = left;
    }

    root->path = 0;
    return root;
}

static Node *analyseCMP() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          Node *  成功识别并返回节点
     *          NULL    识别失败
     */
    Node *root = (Node *)calloc(1, sizeof(Node));
    if(LE_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\n", sizeof("CMP\n"));
        memcpy(root->value, "<=\n", sizeof("<=\n"));
        root->path = 1;
        lexicallyAnalyse();
    } else if(GE_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\n", sizeof("CMP\n"));
        memcpy(root->value, ">=\n", sizeof(">=\n"));
        root->path = 0;
        lexicallyAnalyse();
    } else if(EQ_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\n", sizeof("CMP\n"));
        memcpy(root->value, "==\n", sizeof("==\n"));
        root->path = 2;
        lexicallyAnalyse();
    } else if(GT_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\n", sizeof("CMP\n"));
        memcpy(root->value, ">\n", sizeof(">\n"));
        root->path = 3;
        lexicallyAnalyse();
    } else if(LT_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\n", sizeof("CMP\n"));
        memcpy(root->value, "<\n", sizeof("<\n"));
        root->path = 4;
        lexicallyAnalyse();
    } else {
        throwError("CMP", "分析比较符号时出错");
        return NULL;
    }

    return root;
}

static Node *analyseAddExpr() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          Node *  成功识别并返回节点
     *          NULL    识别失败
     */
    Node *left, *right, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    mamcpy(root->name, "add_expr", sizeof("add_expr"));
    left = analyseMulExpr();
    if(NULL == left) {
        throwError("add_expr", "分析mul_expr时出错");
        return NULL;
    }

    root->left = left;
    temp = root;

    while(SUB_ == nToken.code || PLUS_ == nToken.code) {
        // 将运算符‘-’或‘+’放到右节点
        right = (Node *)calloc(1, sizeof(Node));
        right->value = (char *)calloc(2, sizeof(char));

        if(SUB_ == nToken.code) {
            memcpy(right->name, "SUB\n", sizeof("SUB\n"));
            memcpy(right->value, "-\n", sizeof("-\n"));
        } else {
            memcpy(right->name, "PLUS\n", sizeof("PLUS\n"));
            memcpy(right->value, "+\n", sizeof("+\n"));
        }

        // 配置右节点完成，转载右节点
        temp->right = right;
        temp = temp->right;

        lexicallyAnalyse();
        // mul_expr
        left = analyseMulExpr();

        if(NULL == left) {
            throwError("add_expr", "分析mul_expr时出错");
            return NULL;
        }

        // 判断成功，转载节点
        temp->left = left;
    }

    root->path = 0;
    return root;
}

static Node *analyseMulExpr() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          Node *  成功识别并返回节点
     *          NULL    识别失败
     */
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "mul_expr", sizeof("mul_expr"));
    Node *left, *right, *temp;
    if(SUB_ == nToken.code){
        left = (Node *)calloc(1, sizeof(Node));
        memcpy(right->name, "SUB\n", sizeof("SUB\n"));
        memcpy(right->value, "-\n", sizeof("-\n"));
        root->left = left;
        lexicallyAnalyse();
    }
    temp = root->left;
    while(SUB_ == nToken.code) { //吃掉多余负号
        left = (Node *)calloc(1, sizeof(Node));
        memcpy(left->name, "SUB\n", sizeof("SUB\n"));
        memcpy(left->value, "-\n", sizeof("-\n"));
        temp->next = left;
        temp = temp->next;
        lexicallyAnalyse();
    }
    right = analysePrimaryExpr();
    if(NULL == right) {
        throwError("add_expr", "分析primary_expr时出错");
        return NULL;
    }
    root->right = right;
    temp = right;
    while(MUL_ == nToken.code || DIV_ == nToken.code || PERCENT_ == nToken.code) {
        // 将运算符‘-’或‘+’放到右节点
        left = (Node *)calloc(1, sizeof(Node));
        left->value = (char *)calloc(2, sizeof(char));

        if(MUL_ == nToken.code) {
            memcpy(left->name, "MUL\n", sizeof("MUL\n"));
            memcpy(left->value, "*\n", sizeof("*\n"));
        } else if (DIV_ == nToken.code){
            memcpy(left->name, "DIV\n", sizeof("DIV\n"));
            memcpy(left->value, "\\\n", sizeof("\\\n"));
        }else{
            memcpy(left->name, "PERCENT\n", sizeof("PERCENT\n"));
            memcpy(left->value, "%\n", sizeof("%\n"));
        }
        // 配置右节点完成，转载右节点
        temp->left = left;
        lexicallyAnalyse();
        // primary_expr
        right = analysePrimaryExpr());
        if(NULL == right) {// Failed
            throwError("add_expr", "分析primary_expr时出错");
            return NULL;
        }
        // Succeed
        temp->right = right;
        temp = temp->right;
    }

    return root;
}

static Node *analysePrimaryExpr() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          Node *  成功识别并返回节点
     *          NULL    识别失败
     */
    Node *root;
    Node *left, *right, *temp;
    // root configuration
    root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "primary_expr\n", strlen("primary_expr\n"));
    // start to judge
    if(NUMBER_ == nToken.code || STRING_ == nToken.code) { //判断数字与字符串
        // 将运算符‘-’或‘+’放到右节点
        right = (Node *)calloc(1, sizeof(Node));
        right->value = (char *)calloc(strlen(nToken.value), sizeof(char));
        if(NUMBER_ == nToken.code) {
            memcpy(right->name, "NUMBER\n", strlen("NUMBER\n"));
            memcpy(right->value, nToken.value, strlen(nToken.value));
        } else {
            memcpy(right->name, "STRING\n", strlen("STRING\n"));
            memcpy(right->value, nToken.value, strlen(nToken.value));
        }
        // 配置右节点完成，转载右节点
        root->right = right;
        lexicallyAnalyse();
    } else if(LP_ == nToken.code) { //判断到左括号
        lexicallyAnalyse(); // do not record
        left = analyseExpr();
        if(NULL == left) {
            throwError("primary_expr", "格式'( expr )'错误");
            return NULL;
        }
        root->left = left;
        if(RP_ == nToken.code)
            lexicallyAnalyse();
        else{
            throwError("primary_expr", "格式'( expr )'错误");
            return NULL;
        }
    } else if(ID_ == nToken.code) { //判断到ID
        left = (Node *)calloc(1, sizeof(Node));
        memcpy(left->name, "ID\n", strlen("ID\n"));
        memcpy(left->value, nToken.value, strlen(nToken.value));
        lexicallyAnalyse();
        root->left = left;
        if(LP_ == nToken.code) { ///检测到左括号
            lexicallyAnalyse();
            if(RP_ == nToken.code) { ////若为ID '(' ')'，直接返回true
                lexicallyAnalyse();
                return root;
            }
            right = analyseExprList();
            if(NULL == right) { ////再判断expr_list
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return NULL;
            }
            root->right = right;
            if(RP_ != nToken.code) { ///检测到右括号
                throwError("primary_expr", "格式ID '(' expr_list ')'丢失右括号");
                return NULL;
            }
            lexicallyAnalyse();
        } else if(ASSIGNOP_ == nToken.code) { ///检测到等于号
            // store the '='
            right = (Node *)calloc(1, sizeof(Node));
            right->value = (Node *)calloc(2, sizeof(char));
            memcpy(right->name, "EQUAL\n", strlen("EQUAL\n"));
            memcpy(right->value, "=\n", 2*sizeof(char));
            root->right = right;
            lexicallyAnalyse();
            //judge the expr_list
            left = analyseExprList();
            if(NULL == left) {
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return NULL;
            }
            root->right->left = left;
        } else if(LB_ == nToken.code) { ///检测到左中括号
            lexicallyAnalyse();
            right = analyseExpr();
            if(NULL == right) { ////再判断expr
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return false;
            }
            root->right = right;
            if(RB_ != nToken.code) { ////再判断右中括号
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return NULL;
            }
            lexicallyAnalyse();////已经匹配了右中括号,读入下一个
            if(ASSIGNOP_ == nToken.code) { /* ！发现异常！ */
                // store the '='
                left = (Node *)calloc(1, sizeof(Node));
                left->value = (Node *)calloc(2, sizeof(char));
                memcpy(left->name, "EQUAL\n", strlen("EQUAL\n"));
                memcpy(left->value, "=\n", 2*sizeof(char));
                root->right->left = left;
                /* 若发现=，但后不跟expr，就会报错。但也有可能=是其他的 */
                lexicallyAnalyse();
                right = analyseExpr();
                if(NULL == right) {
                    throwError("primary_expr", "格式ID '[' expr ']' '=' expr错误");
                    return NULL;
                }
                root->right->right = right;
            }
        }///判断到ID的内部if
    } else { //没有匹配项目
        throwError("primary_expr", "分析primay_expr时无匹配格式");
        return NULL;
    }//结束
    return root;
}


static Node *analyseIdList() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          Node *  成功识别并返回节点
     *          NULL    识别失败
     */
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "id_list\n", strlen("id_list\n"));
    Node *next, *temp;
    if(nToken.code == ID_ ) {
        next = (Node *)calloc(1, sizeof(Node));
        memcpy(next->name, "ID\n", strlen("ID\n"));
        memcpy(next->value, nToken.value, strlen(nToken.value));
        root->left = next;
        lexicallyAnalyse();
        // configure the temp
        temp = root->left;
        while(nToken.code == COMMA_) { // ',' COMMA_
            lexicallyAnalyse();
            if(nToken.code != ID_) {
                throwError("id_list", "分析ID时出错");
                return false;
            }
            next = (Node *)calloc(1, sizeof(Node));
            memcpy(next->name, "ID\n", strlen("ID\n"));
            memcpy(next->value, nToken.value, strlen(nToken.value));
            temp->next = next;
            temp = temp->next;
        }
        return root;
    } else {
        throwError("id_list", "无匹配出错");
        return NULL;
    }
}

static Node *analyseExprList() {
    /*
     * @author: 丁志鹏
     * @para: None
     * @return: bool
     *          Node *  成功识别并返回节点
     *          NULL    识别失败
     */
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "expr_list\n", strlen("expr_list\n"));
    Node *next, *temp;
    next = analyseExpr();
    if(NULL != next) {
        root ->left = next;
        // temp configuration
        temp = next;
        while(nToken.code == COMMA_) { // ',' COMMA_
            lexicallyAnalyse();
            next = analyseExpr();
            if(NULL == next) {
                throwError("expr_list", "分析expr时出错");
                return false;
            }
            temp->next = next;
            temp = temp->next;
        }
        return root;
    } else {
        throwError("id_list", "无匹配出错");
        return NULL;
    }
}






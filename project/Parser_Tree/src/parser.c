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

bool isEndOfFile(){
    if(EOF == nToken.code) {
        return true;
    }else{
        return false;
    }
}

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
    * @author: 黄粤升
    * @para: None
    * @return: bool
    *          Node *    成功识别Program
    *          NULL      不符合Program
    */
    // 声明变量
    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    // 初始化变量
    memcpy(root->name, "program\0", strlen("program\0"));

    next = analyseExternalDeclaration();
    temp = next;
    if(next != NULL) {                              // 判断了至少一个external_declaration
        root->left = next;
        while(nToken.code != EOF){
            next = analyseExternalDeclaration();
            if(next != NULL){
                temp->next = next;
                temp = next;
            }
            else{
                throwError("program", "分析external_declaration时出错");
                return NULL;
            }
        }
        return root;
    } else {                                         // 一个external_declaration都没有：抛出错误
        throwError("program", "分析external_declaration时出错");
        return NULL;
    }
    return root;
}


static Node *analyseDeclarator() {
    /*
    * @author: 丁志鹏
    * @para: None
    * @return: bool
    *          Node *    成功识别Program
    *          NULL      不符合Program
    */
    Node *root = (Node *)calloc(1, sizeof(Node));
    Node *left, *right, *temp, *next;
    // 配置root节点
    memcpy(root->name, "declaration\0", strlen("declaration\0"));
    if(ID_ != nToken.code) {
        throwError("declarator", "匹配ID失败");
        return NULL;
    }
    // 配置left节点，存储ID
    left = (Node *)calloc(1, sizeof(Node));
    left->value = (char *)calloc(strlen(nToken.value), sizeof(char));
    memcpy(left->name, "ID\0", strlen("ID\0"));
    memcpy(left->value, nToken.value, strlen(nToken.value));
    lexicallyAnalyse();
    root->left = left;
    // 已经装载左节点（ID）。后续匹配开始
    if(ASSIGNOP_ == nToken.code) { // =
        lexicallyAnalyse();
        //分析expr
        right = analyseExpr();
        if(NULL == right) {
            throwError("declarator", "匹配expr时失败");
            return NULL;
        }
        root->right = right;
        root->path = 1;
    } else if(LP_ == nToken.code) { // ID '('
        lexicallyAnalyse();

        if(RP_ == nToken.code) { // ID '(' ')' -> 返回
            lexicallyAnalyse();
            root->path = 3;
            return root;
        }

        right = analyseParameterList();

        if(NULL == right) {
            throwError("declarator", "匹配parameter_list失败");
            return NULL;
        }

        root->right = right;

        if(RP_ != nToken.code) {
            throwError("declarator", "格式ID '(' expr_list ')'丢失右括号");
            return NULL;
        }

        lexicallyAnalyse();
        root->path = 2;
    } else if(LB_ == nToken.code) { // ID '['
        lexicallyAnalyse();

        if(RB_ == nToken.code) { // ID '[' ']'
            lexicallyAnalyse();

            // 判断是否有‘=’
            if(ASSIGNOP_ == nToken.code) {
                lexicallyAnalyse();
                // 先载入一个intstr_list
                left = analyseIntstrList();

                if(NULL == right) {
                    throwError("declarator", "分析intstr_list时出错");
                    return NULL;
                }

                temp = root;
                temp->right = (Node *)calloc(1, sizeof(Node));
                temp = temp->right;
                temp->path = NULLPATH;
                temp->left = left;
                left = analyseIntstrList();

                while(NULL != next) {
                    temp->right = (Node *)calloc(1, sizeof(Node));
                    temp = temp->right;
                    temp->path = NULLPATH;
                    temp->left = left;
                    left = analyseIntstrList();
                }

                root->path = 5;
            } else {
                root->path = 7;
            }
        }  else { // ID '[' expr ']'
            left = analyseExpr();

            if(NULL == right) {
                throwError("declarator", "分析expr时报错");
                return NULL;
            }

            temp = root;
            temp->right = (Node *)calloc(1, sizeof(Node));
            temp = temp->right;
            temp->path = NULLPATH;
            temp->left = left;
            left = analyseIntstrList();

            if(RB_ != nToken.code) {
                throwError("declarator", "格式ID '[' expr ']'丢失右括号");
                return NULL;
            }

            lexicallyAnalyse();

            // 判断是否有‘=’
            if(ASSIGNOP_ == nToken.code) {
                lexicallyAnalyse();
                // 先载入一个intstr_list
                left = analyseIntstrList();

                if(NULL == right) {
                    throwError("declarator", "分析intstr_list时出错");
                    return NULL;
                }

                temp->right = (Node *)calloc(1, sizeof(Node));
                temp = temp->right;
                temp->path = NULLPATH;
                temp->left = left;
                left = analyseIntstrList();

                while(NULL != next) {
                    temp->right = (Node *)calloc(1, sizeof(Node));
                    temp = temp->right;
                    temp->path = NULLPATH;
                    temp->left = left;
                    left = analyseIntstrList();
                }

                root->path = 4;
            } else {
                root->path = 6;
            }
        }
    }else if(EOF == nToken.code){
         //无匹配项目 ID
        root->path = 0;
    }
    return root;
}

static Node *analyseExternalDeclaration() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: Node
     *          Node* 成功识别external_declaration
     *          NULL  不符合external_declaration
     */
    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "external_declaration\0", strlen("external_declaration\0"));
    next = analyseType();
    if(EOF == nToken.code){
        return NULL;
    }
    if(next != NULL) {
        root->left = next; // Type 放入root的左节点
        next = analyseDeclarator();
        if(next != NULL) {
            temp = (Node *)calloc(1, sizeof(Node)); // 构造虚拟结点temp
            temp->path = NULLPATH; // 标记虚拟结点
            root->right = temp; // 先将虚拟结点放入root的右结点
            temp->left = next; // 再将next 放入虚拟结点的左节点
            next = analyseDeclOrStmt();
            if(next != NULL) {
                temp->right = next; // 最后一个结点，放入虚拟结点的右结点
                root->path = 0;
                return root;
            } else {
                throwError("external_declaration", "分析decl_or_stmt时出错");
                return NULL;
            }
        } else {
            throwError("external_declaration", "分析declarator时出错");
            return NULL;
        }
    } else {
        throwError("external_declaration", "分析type时出错");
        return NULL;
    }
}

static Node *analyseDeclOrStmt() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: Node
    *          Node* 成功识别decl_or_stmt
    *          NULL  不符合decl_or_stmt
    */
    Node *next;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "decl_or_stmt\0", strlen("decl_or_stmt\0"));

    if(nToken.code == LC_) { // '{'
        lexicallyAnalyse();

        if(nToken.code == RC_) { // '}'
            lexicallyAnalyse();
            root->path = 1;
            return root; // ？为空的时候返回什么
        } else if((next = analyseStatementList()) != NULL) {
            root->left = next; // 放入root的左节点

            if(nToken.code == RC_) { // '}'
                lexicallyAnalyse();
                root->path = 0;
                return root;
            } else {
                throwError("decl_or_stmt", "分析'}'时出错");
                return NULL;
            }
        } else {
            throwError("decl_or_stmt", "结构错误");
            return NULL;
        }
    } else if(nToken.code == COMMA_) { //','
        lexicallyAnalyse();

        if((next = analyseDeclaratorList()) != NULL) {
            root->left = next; // 放入root的左节点

            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                root->path = 2;
                return root;
            } else {
                throwError("decl_or_stmt", "分析';'时出错");
                return NULL;
            }
        } else {
            throwError("decl_or_stmt", "分析declarator_list时出错");
            return NULL;
        }
    } else if(nToken.code == SEM_) { // ';'
        lexicallyAnalyse();
        root->path = 3;
        return root; // ？为空的时候返回什么
    } else {
        throwError("decl_or_stmt", "结构错误");
        return NULL;
    }
}

static Node *analyseDeclaratorList() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: Node
    *          Node*    成功识别DeclaratorList
    *          NULL     不符合DeclaratorList
    */

    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "declarator_list\0", strlen("declarator_list\0"));
    next = analyseDeclarator();
    temp = next;
    if(next != NULL) {
        root->left = next; // declarator 作为root的左结点
        while(nToken.code == COMMA_) { //','
            lexicallyAnalyse();
            next = analyseDeclarator();
            if(next == NULL) {
                throwError("declarator_list", "分析declarator时出错");
                return NULL;
            }
            temp->next = next;
            temp = next;
        }

        root->path = 0;
        return root;
    } else {
        throwError("declarator_list", "分析declarator时出错");
        return NULL;
    }
}

static Node *analyseIntstrList() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: Node
    *          Node*    成功识别InstrList
    *          NULL     不符合InstrList
    */

    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "intstr_list", sizeof("intstr_list"));
    next = analyseInitializer();
    temp = next;

    if(next != NULL) {
        root->left = next;

        while(nToken.code == COMMA_) { //','
            lexicallyAnalyse();
            next = analyseInitializer();

            if(next == NULL) {
                throwError("intstr_list", "分析initializer时出错");
                return NULL;
            }

            temp->next = next;
            temp = next;
        }

        root->path = 0;
        return root;
    } else {
        throwError("intstr_list", "分析initializer时出错");
        return NULL;
    }
}

static Node *analyseInitializer() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: Node
    *          Node*    成功识别Initializer
    *          NULL     不符合Initializer
    */
    Node *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "initializer\0", strlen("initializer\0"));

    if(nToken.code == NUMBER_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "NUMBER\0", strlen("NUMBER\0"));
        root->left = temp; // NUMBER 作为root的左结点
        temp->value = (char *)calloc(strlen(nToken.value), sizeof(char));
        temp->value = nToken.value;
        lexicallyAnalyse();
        root->path = 0;
        return root;
    } else if( nToken.code == STR_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "STRING\0", strlen("STRING\0"));
        root->left = temp; // STRING 作为root的左结点
        temp->value = (char *)calloc(strlen(nToken.value), sizeof(char));
        temp->value = nToken.value;
        lexicallyAnalyse();
        root->path = 1;
        return root;
    } else {
        throwError("initializer", "分析NUMBER或STRING时出错");
        return NULL;
    }
}

static Node *analyseParameterList() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: Node
    *          Node*    成功识别ParameterList
    *          NULL     不符合ParameterList
    */

    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "parameter_list\0", strlen("parameter_list\0"));

    next = analyseParameter();
    temp = next;

    if(next != NULL) {
        root->left = next;
        while(nToken.code == COMMA_) { // ',' COMMA_
            lexicallyAnalyse();
            next = analyseParameter();

            if(next == NULL) {
                throwError("parameter_list", "分析','时出错");
                return NULL;
            }

            temp->next = next;
            temp = next;
        }

        root->path = 0;
        return root;
    } else {
        throwError("parameter_list", "结构错误");
        return NULL;
    }
}

static Node *analyseParameter() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: Node
    *          Node*    成功识别Parameter
    *          NULL     不符合Parameter
    */
    Node *next;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "parameter\0", strlen("parameter\0"));
    next = analyseType();

    if(next != NULL) {
        root->left = next;

        if(nToken.code == ID_) {
            next = (Node *)calloc(1, sizeof(Node));
            memcpy(next->name, "ID\0", strlen("ID\0"));
            next->value = (char *)calloc(sizeof(nToken.value), sizeof(char));
            next->value = nToken.value;
            lexicallyAnalyse();
            root->path = 0;
            root->right = next;
            return root;
        } else {
            throwError("parameter", "分析ID时出错");
            return NULL;
        }
    } else {
        throwError("parameter", "结构错误");
        return NULL;
    }
}

static Node *analyseType() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: Node
    *          Node*    成功识别Type
    *          NULL     不符合Type
    */
    Node *root = (Node *)calloc(1, sizeof(Node));
    Node *temp;
    memcpy(root->name, "type", sizeof("type"));
    if(nToken.code == INT_ ) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "INT\0", strlen("INT\0"));
        root->left = temp; // INT 作为root的左结点
        lexicallyAnalyse();
        root->path = 0;
        return root;
    } else if(nToken.code == STR_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "STR\0", strlen("STR\0"));
        root->left = temp; // STR 作为root的左结点
        lexicallyAnalyse();
        root->path = 1;
        return root;
    } else if(nToken.code == VOID_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "VOID\0", strlen("VOID\0"));
        root->left = temp; // VOID 作为root的左结点
        lexicallyAnalyse();
        root->path = 2;
        return root;
    } else if(ID_ == nToken.code){
        root->path = NULLPATH;
        return root;
    } else {
        throwError("type", "结构错误");
        return NULL;
    }
}

static Node *analyseStatement() {
    /*
    * @author: 黄粤升
    * @para: None
    * @return: Node
    *          Node*    成功识别Statement
    *          NULL     不符合Statement
    */

    Node *next, *temp, *temp2;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "statement\0", strlen("statement\0"));

    if(nToken.code == LC_) { // '{' LC
        lexicallyAnalyse();
        next = analyseStatementList();

        if(next != NULL) {
            root->left = next; //statemnt作为root的左结点

            if(nToken.code == RC_) { // '}' RC
                lexicallyAnalyse();
                root->path = 1;
                return root;
            } else {
                throwError("statement", "分析'}'时出错");
                return NULL;
            }
        } else {
            throwError("statement", "分析statement时出错");
            return NULL;
        }
    } else if(nToken.code == IF_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "IF\0", strlen("IF\0"));
        root->left = temp; // IF 作为root的左结点
        temp2 = temp;
        lexicallyAnalyse();

        if(nToken.code == LP_) { // '(' LP
            lexicallyAnalyse();
            next = analyseExpr();

            if(next != NULL) {
                temp->left = next; // expr 作为IF的左结点

                if(nToken.code == RP_) { // ')' RB
                    lexicallyAnalyse();
                    next = analyseStatement();

                    if(next != NULL) {
                        temp->right = next; // statement 作为IF的右结点

                        if(nToken.code == ELSE_) {
                            temp = (Node *)calloc(1, sizeof(Node));
                            memcpy(temp->name, "ELSE\0", strlen("ELSE\0"));
                            temp2->next = temp; // ELSE 作为IF的后续结点
                            temp2 = temp;
                            lexicallyAnalyse();
                            next = analyseStatement();

                            if(next != NULL) {
                                temp->left = next; // statemnt 作为ELSE的左结点
                                root->path = 4;
                                return root;
                            } else {
                                throwError("statement", "分析statement时出错");
                                return NULL;
                            }
                        } else {
                            throwError("statement", "分析ELSE时出错");
                            root->path = 3;
                            return root; // statement 后面允许不接其他东西
                        }
                    } else {
                        throwError("statement", "分析statement时出错");
                        return NULL;
                    }
                } else {
                    throwError("statement", "分析')'时出错");
                    return NULL;
                }
            } else {
                throwError("statement", "分析expr时出错");
                return NULL;
            }
        } else {
            throwError("statement", "分析'('时出错");
            return NULL;
        }
    } else if(nToken.code == WHILE_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "WHILE\0", strlen("WHILE\0"));
        root->left = temp; // WHILE 作为root的左结点
        lexicallyAnalyse();

        if(nToken.code == LP_) { // '('
            lexicallyAnalyse();
            next = analyseExpr();

            if(next != NULL) {
                temp->left = next; // expr 作为WHILE的左结点

                if(nToken.code == RP_) { // ')'
                    lexicallyAnalyse();
                    next = analyseStatement();

                    if(next != NULL) {
                        temp->right = next; // statement作为WHILE的右结点
                        root->path = 5;
                        return root;
                    } else {
                        throwError("statement", "分析statement时出错");
                        return NULL;
                    }
                } else {
                    throwError("statement", "分析')'时出错");
                    return NULL;
                }
            } else {
                throwError("statement", "分析expr时出错");
                return NULL;
            }
        } else {
            throwError("statement", "分析'('时出错");
            return NULL;
        }
    } else if(nToken.code == RETURN_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "RETURN\0", strlen("RETURN\0"));
        root->left = temp; // RETURN 作为root的左结点
        lexicallyAnalyse();

        if(nToken.code == SEM_) { // ';'
            lexicallyAnalyse();
            root->path = 6;
            return root;
        } else if((next = analyseExpr()) != NULL) {
            temp->left = next; // expr 作为 RETURN 的左结点

            if(nToken.code == SEM_) { // ';'
                lexicallyAnalyse();
                root->path = 7;
                return root;
            } else {
                throwError("statement", "分析';'时出错");
                return NULL;
            }
        } else {
            throwError("statement", "分析expr时出错");
            return NULL;
        }
    } else if(nToken.code == PRINT_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "PRINT\0", strlen("PRINT\0"));
        root->left = temp; // PRINT 作为root的左结点
        lexicallyAnalyse();

        if(nToken.code == SEM_) { //';'
            lexicallyAnalyse();
            root->path = 8;
            return root;
        } else if((next = analyseExprList()) != NULL) {
            temp->left = next; // expr_list 作为PRINT 的左结点

            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                root->path = 9;
                return root;
            } else {
                throwError("statement", "分析';'时出错");
                return NULL;
            }
        } else {
            throwError("statement", "分析expr_list时出错");
            return NULL;
        }
    } else if(nToken.code == SCAN_) {
        temp = (Node *)calloc(1, sizeof(Node));
        memcpy(temp->name, "SCAN\0", strlen("SCAN\0"));
        root->left = temp; // SCAN 作为root的左结点
        lexicallyAnalyse();
        next = analyseIdList();

        if(next != NULL) {
            temp->left = next;

            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                root->path = 10;
                return root;
            } else {
                throwError("statement", "分析';'时出错");
                return NULL;
            }
        } else {
            throwError("statement", "分析id_list时出错");
            return NULL;
        }
    } else if((next = analyseType()) != NULL && next->path != NULLPATH) {
        root->left = next; // Type 作为root的左结点
        next = analyseDeclaratorList();

        if(next != NULL) {
            root->right = next; // declartor_list 作为root的右结点

            if(nToken.code == SEM_) { //';'
                lexicallyAnalyse();
                root->path = 0;
                return root;
            } else {
                throwError("statement", "分析';'时出错");
                return NULL;
            }
        } else {
            throwError("statement", "分析declarator_list时出错");
            return NULL;
        }
    } else if((next = analyseExprStatement()) != NULL) {
        root->left = next; // expr_statement 作为root的左结点
        root->path = 2;
        return root;
    } else {
        throwError("statement", "结构错误");
        return NULL;
    }
}

static Node *analyseStatementList() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: Node
     *          Node*    成功识别IntstrList
     *          NULL     不符合IntstrList
     */
    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "statement_list\0", strlen("statement_list\0"));
    next = analyseStatement();
    temp = next;

    if(next != NULL) {
        root->left = next; // statemnt_list 作为root的左结点

        while(nToken.code != RC_) { //'}' 用右括号作为停止条件
            next = analyseStatement();

            if(next == NULL) {
                throwError("statement_list", "分析statement时出错");
                return NULL;
            }

            temp->next = next;
            temp = next;
        }

        root->path = 0;
        return root;
    } else {
        throwError("statement_list", "结构错误");
        return NULL;
    }
}

static Node *analyseExprStatement() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: Node
     *          Node*    成功识别ExprStatement
     *          NULL     不符合ExprStatement
     */

    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "expr_statement\0", strlen("expr_statement\0"));

    if(nToken.code == SEM_) { //';'
        lexicallyAnalyse();
        root->path = 0;
        return root; // 空节点
    } else if((next = analyseExpr()) != NULL) {
        root->left = next; // expr 作为root的左结点

        if(nToken.code == SEM_) { //';'
            lexicallyAnalyse();
            root->path = 1;
            return root;
        } else {
            throwError("expression_statement", "分析';'时出错");
            return NULL;
        }
    } else {
        throwError("expression_statement", "结构错误");
        return NULL;
    }
}

static Node *analyseExpr() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: Node
     *          Node*     成功识别Expr
     *          NULL      不符合Expr
     */
    Node *next;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "expr\0", sizeof("expr\0"));
    next = analyseCmpExpr();

    if(next != NULL) {
        root->left = next; // cmp_expr 作为root的左结点
        root->path = 0;
        return root;
    } else {
        throwError("expr", "结构错误");
        return NULL;
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
    Node *next;
    Node *root = (Node *)calloc(1, sizeof(Node));
    // 初始化变量
    memcpy(root->name, "cmp_expr\0", strlen("cmp_expr\0"));
    next = analyseAddExpr();

    if(next != NULL) {
        root->left = next; // add_expr作为root的左结点
        root->path = 0;
        next = analyseCMP();

        if(next != NULL) {
            root->path = next->path;
            next = analyseCmpExpr();

            if(next != NULL) {
                root->right = next;
                return root;
            } else {
                throwError("cmp_expr", "分析cmp_expr时出错");
            }
        }

        return root;
    } else {
        throwError("cmp_expr", "分析add_expr时出错");
        return NULL;
    }
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
        memcpy(root->name, "CMP\0", strlen("CMP\0"));
        memcpy(root->value, "<=\0", strlen("<=\0"));
        root->path = 2;
        lexicallyAnalyse();
    } else if(GE_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\0", strlen("CMP\0"));
        memcpy(root->value, ">=\0", strlen(">=\0"));
        root->path = 1;
        lexicallyAnalyse();
    } else if(EQ_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\0", strlen("CMP\0"));
        memcpy(root->value, "==\0", strlen("==\0"));
        root->path = 3;
        lexicallyAnalyse();
    } else if(GT_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\0", strlen("CMP\0"));
        memcpy(root->value, ">\0", strlen(">\0"));
        root->path = 4;
        lexicallyAnalyse();
    } else if(LT_ == nToken.code) {
        root->value = (char *)calloc(3, sizeof(char));
        memcpy(root->name, "CMP\0", strlen("CMP\0"));
        memcpy(root->value, "<\0", strlen("<\0"));
        root->path = 5;
        lexicallyAnalyse();
    } else {
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
    Node *next;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "add_expr\0", strlen("add_expr\0"));
    next = analyseMulExpr();

    if(next != NULL) {
        root->left = next;
        root->path = 0;

        if(nToken.code == PLUS_) {
            lexicallyAnalyse();
            next = analyseAddExpr();

            if(next != NULL) {
                root->right = next;
                root->path = 1;
            } else {
                throwError("add_expr", "分析add_expr时出错");
                return NULL;
            }
        } else if(nToken.code == SUB_) {
            lexicallyAnalyse();
            next = analyseAddExpr();

            if(next != NULL) {
                root->right = next;
                root->path = 2;
            } else {
                throwError("add_expr", "分析add_expr时出错");
                return NULL;
            }
        }

        return root;
    } else {
        throwError("add_expr", "分析mul_expr时出错");
    }

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
    memcpy(root->name, "mul_expr\0", strlen("mul_expr\0"));
    Node *left, *right, *temp;

    if(SUB_ == nToken.code) {
        lexicallyAnalyse();
        root->path = 5;
    }

    left = analysePrimaryExpr();

    if(NULL == left) {
        throwError("add_expr", "分析primary_expr时出错");
        return NULL;
    }

    root->left = left;

    if(MUL_ == nToken.code) {
        lexicallyAnalyse();
        right = analysePrimaryExpr();

        if(NULL == right) {
            throwError("add_expr", "* 分析primary_expr时出错");
            return NULL;
        }

        root->right = right;
        right->path = 1;
    } else if(DIV_ == nToken.code) {
        lexicallyAnalyse();
        right = analysePrimaryExpr();

        if(NULL == right) {
            throwError("add_expr", "/ 分析primary_expr时出错");
            return NULL;
        }

        root->right = right;
        right->path = 2;
    } else if(PERCENT_ == nToken.code) {
        lexicallyAnalyse();
        right = analysePrimaryExpr();

        if(NULL == right) {
            throwError("add_expr", "% 分析primary_expr时出错");
            return NULL;
        }

        root->right = right;
        right->path = 3;
    } else {
        if(root->path != 5) {
            root->path = 1;
        }
    }

    return root;
}

/* TODO: 检查primary_expr函数。逻辑可能有误。包括不构建树的项目 */
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
    memcpy(root->name, "primary_expr\0", strlen("primary_expr\0"));
    // start to judge
    if(NUMBER_ == nToken.code || STRING_ == nToken.code) { //判断数字与字符串
        left = (Node *)calloc(1, sizeof(Node));
        left->value = (char *)calloc(strlen(nToken.value), sizeof(char));
        if(NUMBER_ == nToken.code) {
            memcpy(left->name, "NUMBER\0", strlen("NUMBER\0"));
            memcpy(left->value, nToken.value, strlen(nToken.value));
            root->path = 7;
        } else {
            memcpy(left->name, "STRING\0", strlen("STRING\0"));
            memcpy(left->value, nToken.value, strlen(nToken.value));
            root->path = 8;
        }
        // 配置左节点完成，转载左节点
        root->left = left;
        lexicallyAnalyse();
    } else if(LP_ == nToken.code) { //判断到左括号
        lexicallyAnalyse(); // do not record
        left = analyseExpr();

        if(NULL == left) {
            throwError("primary_expr", "格式'( expr )'错误");
            return NULL;
        }

        root->left = left;
        root->path = 6;

        if(RP_ == nToken.code)
            lexicallyAnalyse();
        else {
            throwError("primary_expr", "格式'( expr )'错误");
            return NULL;
        }
    } else if(ID_ == nToken.code) { //判断到ID
        left = (Node *)calloc(1, sizeof(Node));
        left->value = (char *)calloc(strlen(nToken.value), sizeof(char));
        memcpy(left->name, "ID\0", strlen("ID\0"));
        memcpy(left->value, nToken.value, strlen(nToken.value));
        lexicallyAnalyse();
        root->left = left;
        root->path = 2;

        if(LP_ == nToken.code) { ///检测到左括号
            lexicallyAnalyse();

            if(RP_ == nToken.code) { ////若为ID '(' ')'，直接返回true
                lexicallyAnalyse();
                root->path = 1;
                return root;
            }

            right = analyseExprList();

            if(NULL == right) { ////再判断expr_list
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return NULL;
            }

            root->right = right;
            root->path = 0;

            if(RP_ != nToken.code) { ///检测到右括号
                throwError("primary_expr", "格式ID '(' expr_list ')'丢失右括号");
                return NULL;
            }

            lexicallyAnalyse();
        } else if(ASSIGNOP_ == nToken.code) { ///检测到等于号
            //judge the expr_list
            right = analyseExprList();
            if(NULL == right) {
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return NULL;
            }
            root->right = right;
            root->path = 3;
        } else if(LB_ == nToken.code) { ///检测到左中括号
            lexicallyAnalyse();
            right = analyseExpr();

            if(NULL == right) { ////再判断expr
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return NULL;
            }

            root->right = right;

            if(RB_ != nToken.code) { ////再判断右中括号
                throwError("primary_expr", "格式ID '(' expr_list ')'错误");
                return NULL;
            }

            lexicallyAnalyse();////已经匹配了右中括号,读入下一个
            root->path = 4;

            if(ASSIGNOP_ == nToken.code) { /* ！发现异常！ */
                /* 若发现=，但后不跟expr，就会报错。但也有可能=是其他的 */
                lexicallyAnalyse();
                left = analyseExpr();
                if(NULL == left) {
                    throwError("primary_expr", "格式ID '[' expr ']' '=' expr错误");
                    return NULL;
                }
                root->right->left = left;
                root->path  = 5;
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
    memcpy(root->name, "id_list\0", strlen("id_list\0"));
    Node *next, *temp;

    if(nToken.code == ID_ ) {
        next = (Node *)calloc(1, sizeof(Node));
        memcpy(next->name, "ID\0", strlen("ID\0"));
        memcpy(next->value, nToken.value, strlen(nToken.value));
        root->left = next;
        lexicallyAnalyse();
        // configure the temp
        temp = root->left;

        while(nToken.code == COMMA_) { // ',' COMMA_
            lexicallyAnalyse();

            if(nToken.code != ID_) {
                throwError("id_list", "分析ID时出错");
                return NULL;
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
    memcpy(root->name, "expr_list\0", strlen("expr_list\0"));
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
                return NULL;
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

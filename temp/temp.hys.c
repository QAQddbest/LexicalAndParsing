/*
 * 本段语法：
 * external_declaration
 *     : type declarator decl_or_stmt
 *
 * 备注：
 *     待分析函数。分析该函数是否完善
 */
static Node *analyseExternalDeclaration()
{
    /*
       * @author: 黄粤升
       * @para: None
       * @return: Node
       *
       *
       */

    Node *next, *temp;
    Node *root = (Node *)calloc(1, sizeof(Node));
    memcpy(root->name, "external_declaration", sizeof("external_declaration"));

    next = analyseType();

    if(next != NULL) {
        root->left = next; // Type 放入root的左节点
        next = analyseDeclarator();

        if(next != NULL) {
            temp = (Node *)calloc(1, sizeof(Node)); // 构造虚拟结点temp
            temp->path = -1; // 标记虚拟结点
            root->right = temp; // 先将虚拟结点放入root的右结点
            temp->left = next; // 再将next 放入虚拟结点的左节点
            next = analyseDeclOrStmt();

            if(next != NULL) {
                temp->right = next; // 最后一个结点，放入虚拟结点的右结点
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
# 语法分析总括(不构建语法树)

## 变量与结构声明

为了存储富含信息的下条Tokens，定义结构

```c
typedef struct{
	int row;			// 行数
	int col;			// 列数
	int code;			// 该Token的种别码
	char *value;		// 该Token的属性值
}Nexttoken;

```

并实例化该结构，为两个变量

1. `nToken`
2. `nnToken`

## 函数列表

### 非终结符分析函数

#### `bool analyseProgram()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseExternalDeclaration()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseDeclOrStmt()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseDeclaratorList()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseIntstrList()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseInitializer()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### bool analyseDeclarator()

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseParameterList()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseParameter()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseType()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseStatemrnt()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### bool analyseStatementList()

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseExpressionStatement()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### bool analyseExpr()

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseCmpExpr()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### bool analyseAddExpr()

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseMulExpr()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analysePrimaryExpr()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### `bool analyseExprList()`

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

#### bool analyseIdList()

##### 接受参数

无

##### 返回参数

1. 识别成功 返回true

2. 识别失败 返回false

### `void throwError(char *place, char *message)`

#### 接受参数

1. char *place

    出错地址，由程序提供。即语法分析出错语法名称。

2. char *message

    出错信息，由程序

#### 返回参数

无

#### 函数说明

抛出错误信息，并显示错误具体原因，错误发生行数与列数

### `extern void lexicallyAnalyse()`

#### 接受参数

无

#### 返回参数

无

#### 函数说明

外部函数(Flex词法分析接口函数)。用于更新nToken与nnToken的值。

### `extern void initLexer(FILE *lex)`

#### 接受参数

指向待分析rd源文件的文件指针

#### 返回参数

无

#### 函数说明

用于初始化词法分析器。（必须在 `lexicallyAnalyse()` 前调用）

将对`yyin`赋值
并初始化`nToken`，预加载一次


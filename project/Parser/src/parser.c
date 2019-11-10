/*
 * @author: 丁志鹏
 * @date: 2019-11-10
 * @license: MIT
 * @description: 词法分析所需的全部主函数。
 */

bool analyseIntstrList() {
    /*
     * @author: 黄粤升
     * @para: None
     * @return: bool
     *          true    成功识别IntstrList
     *          false   不符合IntstrList
     */
    if(analyseInitializer() == true) {
        while(nextToken == ',') {
            if(analyseInitializer() == false) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool analyseInitializer() {
    if(nextToken == NUMBER || nextToken == STRING) {
        return true;
    } else {
        // error()
        return false;
    }
}

bool analyseDeclarator() {
    if(nextToken == ID) {
        if(nextToken == '=' ) {
            if(analyseExpr() == true) {
                return true;
            } else {
                return false;
            }
        } else if(nextToken == '(' ) {
            if(nextToken == ')' ) {
                return true;
            } else if(analyseParameterList() == true) {
                if(nextToen == ')' ) {
                    return true;
                } else {
                    // error()
                    return false;
                }
            }
        } else if(nextToken == '[' ) {
            if(nextToken == ']') {
                if(nextToken == '=') {
                    if(nextToken == '{') {
                        if(analyseInstrList() == true) {
                            if(nextToken == '}') {
                                return true;
                            } else {
                                // error();
                                return false;
                            }
                        } else {
                            return false;
                        }
                    } else {
                        // error();
                        return false;
                    }
                } else {
                    return true; // ']' ºóÃæÔÊÐí²»¸úÈÎºÎ¶«Î÷
                }
            } else if(analyseExpr() == true) {
                if(nextToken == ']') {
                    if(nextToken == '=') {
                        if(nextToken == '{') {
                            if(analyseInstrList() == true) {
                                if(nextToken == '}') {
                                    return true;
                                } else {
                                    // error();
                                    return false;
                                }
                            } else {
                                return false;
                            }
                        } else {
                            // error();
                            return false;
                        }
                    } else {
                        return true; // ']' ºóÃæÔÊÐí²»¸úÈÎºÎ¶«Î÷
                    }
                } else {
                    // error();
                    return false;
                }
            } else {
                return false;
            }
        } else {
            return true; // ID ºóÃæÔÊÐí²»¸úÈÎºÎ¶«Î÷
        }
    } else {
        // error();
        return false;
    }
}

bool analyseParameterList() {
    if(analyseParameter() == true) {
        while(nextToken == ',') {
            if(analyseParameter() == false) {
                return false;
            }
        }
    } else {
        return false;
    }
}

bool analyseParameter() {
    if(analyseType() == true) {
        if(nextToken == ID) {
            return true;
        } else {
            // error();
            return false;
        }
    } else {
        return false;
    }
}

bool analyseType() {
    if(nextToken == INT || nextToken == STR || nextToken == VOID) {
        return true;
    } else {
        // error()
        return false;
    }
}

bool analyseStatement() {
    if(nextToken == '{') {
        if(analyseStatementList() == true) {
            if(nextToken == '}') {
                return true;
            } else {
                // error()
                return false;
            }
        } else {
            return false;
        }
    } else if(nextToken == IF) {
        if(nextToken == '(') {
            if(analyseExpr() == true) {
                if(nextToken == ')') {
                    if(analyseStatement() == true) {
                        if(nextToken == ELSE) {
                            if(analyseStatement() == true) {
                                return true;
                            } else {
                                return false;
                            }
                        } else {
                            return true; // statement ºóÃæÔÊÐí²»½ÓÆäËû¶«Î÷
                        }
                    } else {
                        return false;
                    }
                } else {
                    // error()
                    return false;
                }
            } else {
                return false;
            }
        } else {
            // error()
            return false;
        }
    } else if(nextToken == WHILE) {
        if(nextToken == '(') {
            if(analyseExpr() == true) {
                if(nextToken == ')') {
                    if(analyseStatement() == true) {
                        return true;
                    } else {
                        return false;
                    }
                } else {
                    // error()
                    return false;
                }
            } else {
                return false;
            }
        } else {
            // error()
            return false;
        }
    } else if(nextToken == RETURN) {
        if(nextToken == ';') {
            return true;
        } else if(analyseExpr() == true) {
            if(nextToken == ';') {
                return true;
            } else {
                // error()
                return false;
            }
        } else {
            return false;
        }
    } else if(nextToken == PRINT) {
        if(nextToken == ';') {
            return true;
        } else if(analyseExprList() == true) {
            if(nextToken == ';') {
                return true;
            } else {
                //    error()
                return false;
            }
        } else {
            return false;
        }
    } else if(nextToken == SCAN) {
        if(analyseIdList() == true) {
            if(nextToken == ';') {
                return true;
            } else {
                // error()
                return false;
            }
        } else {
            return false;
        }
    } else if(analyseType() == true) {
        if(analyseDeclaratorList) {
            if(nextToken == ';') {
                return true;
            } else {
                // error()
                return false;
            }
        } else {
            return false;
        }
    } else if(analyseExprStatement() == true) {
        return true;
    } else {
        return false;
    }
}

bool analyseStatementList() {
    if(analyseStatement() == true) {
        while(nextToken != '}') {
            if(analyseStatement() != true) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

bool analyseExprStatement() {
    if(nextToke == ';') {
        return true;
    } else if(analyseExpr() == true) {
        if(nextToken == ';') {
            return true;
        } else {
            // error()
            return false;
        }
    } else {
        return false;
    }
}

bool analyseExpr() {
    if(analyseCmpExpr() == true) {
        return true;
    } else {
        return false;
    }
}



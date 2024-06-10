#pragma once
#include <bits/stdc++.h>
#include "Word.h"
class TreeNode { // 语法分析树节点
public:
    std::string content; // 内容
    std::vector<TreeNode*>children; // 子节点
    TreeNode(){}
    TreeNode(std::string content){this->content = content;}
    ~TreeNode(){}
    TreeNode* newNode(std::string content){
        TreeNode* node = new TreeNode(content); // 新建一个子节点，添加到列表中，并返回这个子节点
        addChild(node);
        return node;
    }
    void addChild(TreeNode* node){
        children.push_back(node);
    }
};
class SyntaxAnalyzer{
private:
    std::vector<Word>words; // 词法分析结果，即词语流
    size_t cur; // 当前分析到第几个词了
    int dep; // 记录嵌套子程序深度
    TreeNode* root; // 语法分析树根节点
public:
    SyntaxAnalyzer(const std::vector<Word>words){
        for(auto word:words){
            this->words.push_back(word);
        }
        root = nullptr;
        dep = cur = 0;
    }
    
    bool parse() { // 语法分析主程序，生成语法分析树或者返回false表示语法错误
        //<程序>→<分程序>.
        if(!(words.size()>0 && cur<words.size())) return false;// 词语流不为空, 且cur指针合法
        root = new TreeNode("PROGRAM"); // 初始化根节点
        bool ok = true;
        ok &= SUBPROG(root); // 处理分程序
        if(words[cur].content == "."){ // 结尾
            root->newNode(".");
            cur++;
        } else ok = false; // 缺少结尾 .
        if(cur<words.size()){ // 结尾之后还有东西
            ok = false;
        }
        return ok;
    }

    bool SUBPROG(TreeNode *node){ // 处理分程序
        // <分程序>→ [<常量说明部分>][<变量说明部分>][<过程说明部分>]<语句>
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        bool ok = true;
        TreeNode *nd=node->newNode("SUBPROG"); 
        if(words[cur].content == "CONST") ok &= CONST(nd); // 常量说明部分
        if(words[cur].content == "VAR")  ok &= VAR(nd); // 变量说明部分
        if(words[cur].content == "PROCEDURE") ok &= PROCEDURE(nd); // 过程说明部分
        ok &= SENTENCE(nd); // 语句
        return ok;
    }
    
    bool CONST(TreeNode *node){ // 常量说明部分
        // <常量说明部分> → CONST<常量定义>{ ,<常量定义>};
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *nd = node->newNode("CONSTANTDECLARE"); // 常量说明部分
        nd->newNode("CONST"), cur++;
        bool ok = true;
        auto CONSTANTDEFINE = [&](){ // 常量定义
            if(words[cur].type == IDENTIFIER && words[cur+1].content == "=" && words[cur+2].type == NUMBER) {
                TreeNode* child = nd->newNode("CONSTANTDEFINE");
                child->newNode(words[cur].content);
                child->newNode(words[cur+1].content);
                child->newNode(words[cur+2].content);
                cur+=3;
            }else return false;
            return true;
        };
        ok &= CONSTANTDEFINE();
        // 下面是连续可能的常量定义
        while(cur < words.size() && words[cur].content == ","){
            nd->newNode("COMMA");
            cur++; 
            ok &= CONSTANTDEFINE();
            if(!ok) return ok;
        }
        if(words[cur].content == ";") nd->newNode(";"), cur++; // 结尾分号
        else ok = false;
        return ok;
    }
    
    bool VAR(TreeNode *node){ // 变量说明部分
        // <变量说明部分> → VAR<标识符>{ ,<标识符>};
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *nd=node->newNode("VARIABLEDECLARE"); // 变量说明部分
        nd->newNode("VAR"), cur++;
        bool ok = true;
        auto VARIABLEDEFINE = [&](){ // 变量定义
            if(words[cur].type == IDENTIFIER){
                nd->newNode(words[cur].content);
                cur++;
            }else return false;
            return true;
        };
        ok &= VARIABLEDEFINE();
        // 下面是连续可能的变量定义 
        while(cur < words.size() && words[cur].content == ","){
            nd->newNode("COMMA"), cur++;
            ok &= VARIABLEDEFINE();
        }
        if(words[cur].content == ";"){ // 结尾分号
            nd->newNode(";"), cur++;
        }else ok = false;
        return ok;
    }

    bool PROCEDURE(TreeNode *node){ // 过程说明部分
        // <过程说明部分> → <过程首部><分程序>;{<过程说明部分>}
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *nd=node->newNode("PROCEDUREDECLARE");
        dep++;
        if(dep>3){ // 超过3层
            return false;
        }
        bool ok = true;
        ok &= HEADOFPROCE(nd); // 过程首部
        ok &= SUBPROG(nd); // 分程序
        dep--;
        if(words[cur].content==";"){
            nd->newNode(";");
            cur++;
        }else return false;
        while(cur < words.size() && words[cur].content=="PROCEDURE"){ // 可能存在的过程说明部分
            ok &= PROCEDURE(nd);
        }
        return ok;
    }

    bool HEADOFPROCE(TreeNode *node){ // 过程首部
        // <过程首部> → PROCEDURE <标识符>;
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *head=node->newNode("PROCEDUREHEAD");
        head->newNode("PROCEDURE"), cur++;
        if(words[cur].type == IDENTIFIER){ // 标识符
            head->newNode(words[cur].content);
            cur++;
        }
        else return false;
        if(words[cur].content == ";"){ // 分号
            head->newNode(";");
            cur++;
        }
        else return false;
        return true;
    } 
    
    bool SENTENCE(TreeNode *node){ // 语句
        // <语句> → <赋值语句>|<条件语句>|<当型循环语句>|<过程调用语句>|<读语句>|<写语句>|<复合语句>|<空语句>
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *nd=node->newNode("SENTENCE");
        bool ok = true;
        if(cur+1 < words.size() && words[cur+1].content == ":=") ok &= ASSIGN(nd); // 赋值
        else if(words[cur].content == "IF")     ok &= IF(nd); // 条件
        else if(words[cur].content == "WHILE")  ok &= WHILE(nd); // 当型循环
        else if(words[cur].content == "CALL")   ok &= CALL(nd); // 过程调用
        else if(words[cur].content == "READ")   ok &= READ(nd); // 读
        else if(words[cur].content == "WRITE")  ok &= WRITE(nd); // 写
        else if(words[cur].content == "BEGIN")  ok &= COMBINED(nd) ; // 复合
        else                                    ok &= EMPTY(nd); // 空
        return ok;
    }

    bool ASSIGN(TreeNode *node){ // 赋值语句
        // <赋值语句> → <标识符>:=<表达式>
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *nd=node->newNode("ASSIGNMENT");
        if(words[cur].type == IDENTIFIER && words[cur+1].content == ":="){
            nd->newNode(words[cur].content);
            nd->newNode(words[cur+1].content);
            cur+=2;
        }
        else return false;
        bool ok = true;
        ok &= EXPRESSION(nd); 
        return ok;
    }
    
    bool COMBINED(TreeNode *node){ // 复合语句
        // <复合语句> → BEGIN<语句>{ ;<语句>} END
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        bool ok = true;
        TreeNode *nd=node->newNode("COMBINED");
        nd->newNode("BEGIN");
        cur++;
        ok &= SENTENCE(nd);
        while(cur < words.size() && words[cur].content==";"){ // 可能存在的连续语句
            nd->newNode(";");
            cur++;
            ok &= SENTENCE(nd);
        }
        if(words[cur].content=="END"){
            nd->newNode("END");
            cur++;
        }
        else return false;
        return ok;
    }

    bool CONDITION(TreeNode *node){ // 条件
        // <条件> → <表达式><关系运算符><表达式>|ODD<表达式>
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        bool ok = true;
        TreeNode *nd=node->newNode("CONDITION");
        if(words[cur].content=="ODD"){ // ODD表达式
            nd->newNode("ODD");
            cur++;
            ok &= EXPRESSION(nd);
        }
        else{
            ok &= EXPRESSION(nd);
            if(words[cur].content == "=" || words[cur].content == "#" || words[cur].content == "<" 
            || words[cur].content == "<=" || words[cur].content == ">" || words[cur].content == ">="){ // 关系运算符
                nd->newNode(words[cur].content);cur++;	
            } else return false;
            ok &= EXPRESSION(nd);
        }
        return ok;
    }

    bool EXPRESSION(TreeNode *node){ // 表达式
        // <表达式> → [+|-]<项>{<加减运算符><项>}
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        bool ok = true;
        TreeNode *nd=node->newNode("EXPRESSION");
        if(words[cur].content == "+" || words[cur].content == "-"){
            nd->newNode(words[cur].content),cur++;
        }
        ok &= ITEM(nd);
        while(cur < words.size() && (words[cur].content == "+" || words[cur].content == "-")){ // 可能出现的多个项
            nd->newNode(words[cur].content),cur++;
            ok &= ITEM(nd);
        }
        return ok;
    }

    bool ITEM(TreeNode *node){ // 项
        // <项> → <因子>{<乘除运算符><因子>}
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        bool ok = true;
        TreeNode *nd = node->newNode("ITEM");
        ok &= FACTOR(nd);
        while(cur < words.size() && (words[cur].content == "*" || words[cur].content == "/")){
            nd->newNode(words[cur].content);
            cur++;
            ok &= FACTOR(nd); 
        }
        return ok;
    }

    bool FACTOR(TreeNode *node){ // 因子
        // <因子> → <标识符>|<无符号整数>|(<表达式>)
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        bool ok = true;
        TreeNode *nd=node->newNode("FACTOR");
        if(words[cur].type == IDENTIFIER || words[cur].type == NUMBER){
            nd->newNode(words[cur].content),cur++;
        }else if(words[cur].content=="("){ // (表达式)
            nd->newNode("LP");
            cur++;
            ok &= EXPRESSION(nd);
            if(words[cur].content==")"){
                nd->newNode("RP");
                cur++;
            }
            else return false;
        }
        else return false;
        return ok;
    }

    bool IF(TreeNode* node){ // 条件语句
        // <条件语句> → IF<条件>THEN<语句>
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        bool ok = true;
        TreeNode *nd = node->newNode("IFSENTENCE");
        nd->newNode("IF");
        cur++;
        ok &= CONDITION(nd);
        if(words[cur].content == "THEN"){
            nd->newNode("THEN");
            cur++;
        }
        else return false;
        ok &= SENTENCE(nd);
        return ok;
    }
    
    bool CALL(TreeNode *node){ // 过程调用语句
        // <过程调用语句> → CALL<标识符>
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *nd = node->newNode("CALLSENTENCE");
        nd->newNode("CALL");
        cur++;
        if(words[cur].type == IDENTIFIER){
            nd->newNode(words[cur].content);
            cur++;
        } else return false;
        return true;
    }

    bool WHILE(TreeNode *node){ // 当型循环语句
        // <当型循环语句> → WHILE<条件>DO<语句>
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        bool ok = true;
        TreeNode *nd = node->newNode("WHILESENTENCE");
        nd->newNode("WHILE");
        cur++;
        ok &= CONDITION(nd);
        if(words[cur].content == "DO"){
            nd->newNode("DO"), cur++;
        } else return false;
        ok &= SENTENCE(nd);
        return ok;
    }
    
    bool READ(TreeNode* node){ // 读
        // <读语句> → READ(<标识符>{ ,<标识符>})
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *nd = node->newNode("READSENTENCE");
        nd->newNode("READ");
        cur++;
        if(words[cur].content == "("){
            nd->newNode("LP");
            cur++;
            if(words[cur].type == IDENTIFIER){
                nd->newNode(words[cur].content), cur++;
            }else return false;
            while(cur < words.size() && words[cur].content == ","){ // 可能出现的多个标识符
                nd->newNode("COMMA"),cur++;
                if(words[cur].type == IDENTIFIER){
                    nd->newNode(words[cur].content), cur++;
                }else return false;
            }
            if(words[cur].content == ")"){
                nd->newNode("RP");cur++;
            }else return false;
        }else return false;
        return true;
    }
    
    bool WRITE(TreeNode *node){ // 写
        // <写语句> → WRITE(<标识符>{,<标识符>})
        assert(words.size()>0 && cur<words.size()); // 词语流不为空, 且cur指针合法
        TreeNode *nd = node->newNode("WRITESENTENCE");
        nd->newNode("WRITE"),cur++;
        if(words[cur].content=="("){
            nd->newNode("LP");
            cur++;
            if(words[cur].type == IDENTIFIER){
                nd->newNode(words[cur].content), cur++;
            }else return false;
            while(words[cur].content == ","){
                nd->newNode("COMMA"),cur++;
                if(words[cur].type == IDENTIFIER){
                    nd->newNode(words[cur].content), cur++;
                }else return false;
            }
            if(words[cur].content == ")"){
                nd->newNode("RP"), cur++;
            }else return false;
        }else return false;
        return true;
    }

    bool EMPTY(TreeNode *node){ // 空语句
        node->newNode("EMPTY");
        return true;
    }
    
    void dfs(TreeNode* node) const { // 递归输出语法分析子树
        std::cout << node->content;
        int n=node->children.size();
        if(n!=0){
            std::cout << "(";
            for(int i = 0;i < n;i++){
                dfs(node->children[i]);
                if(i+1 < n) std::cout << ",";
            }
            std::cout << ")";
        }
    }
    void print() const { // 将语法分析树输出成括号形式
        dfs(root);
    }
};
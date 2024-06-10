#pragma once
#include <bits/stdc++.h>
enum Type{
    NUMBER, // 数字
    IDENTIFIER, // 标识符
    KEYWORD, // 关键词
    DELIMITER, // 分隔符
    OPERATOR, // 运算符
};
class Word{
public:
    std::string content; // 单词内容
    Type type; //单词类型
    Word(){}
    Word(const Word& word) :content(word.content), type(word.type){}
    Word(const std::string &con, Type tp) :content(con), type(tp){}
    void print() const{
        if(type == NUMBER){
            std::cout << "NUMBER ";
        }else if(type == IDENTIFIER){
            std::cout << "IDENTIFIER ";
        }
        std::cout << content;
    }
};
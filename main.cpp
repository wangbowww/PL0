#include <bits/stdc++.h>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
int main(){
    LexicalAnalyzer la;
    std::string s;
    bool ok = true;
    while(getline(std::cin,s)){
        if(!la.parseLine(s)){
            ok = false;
        }
    }
    if(!ok){
        std::cout << "Lexical Error"; // 词法错误
        return 0;
    }
    // 开始语法分析syntax analysis
    SyntaxAnalyzer sa(la.getWords());
    if(!sa.parse()){
        std::cout << "Syntax Error"; // 词法错误
    }else sa.print();
    return 0;
}
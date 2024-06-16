#include <bits/stdc++.h>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
int main(){
    freopen("program.code", "w", stdout);
    LexicalAnalyzer la;
    std::string s;
    bool ok = true;
    while(getline(std::cin,s)){
        if(!la.parseLine(s)){
            ok = false;
        }
    }
    if(!ok){
        return -1;
    }
    // 开始语法分析syntax analysis
    SyntaxAnalyzer sa(la.getWords());
    if(!sa.parse()){
        return -1;
    }else sa.printCode();
    return 0;
}
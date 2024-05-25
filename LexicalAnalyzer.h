#include <bits/stdc++.h>
#include "Word.h"

bool is_operator(char x){
    if(x=='=' || x=='+' || x=='-' || x=='*' ||x=='/' || x=='#' || x=='<' || x=='>' || x==':') return true;
    return false;
}

bool is_delimiter(char x){
    if(x==';' || x==',' || x=='.' || x=='(' || x==')') return true;
    return false;
}

bool is_illegal(char x){ // 非法字符
    if(!is_delimiter(x) && !is_operator(x) && x!=char(9) && x!=char(32) && !isdigit(x) && !isalpha(x)) return true;
    return false;
}

class LexicalAnalyzer{
private:
    std::vector<Word>words;
public:
    bool parseLine(const std::string &str){ // 分析输入的一行
        int n = str.size();
        for(int i=0;i<n;++i){
            //这是某个"单词"的首部
            if(is_illegal(str[i])) return false;
            if(int(str[i])==9 || int(str[i])==32){ // 空格和tab
                continue;
            }
            std::string word="";
            if(isdigit(str[i])){ // 数字
                //认为接下来是一串数字
                int j=i;
                int x=0;
                for(;j<n;++j){
                    if(isdigit(str[j])) x = x*10+str[j]-'0'; // 去除前导0
                    else if(isalpha(str[j])){ // 数字不能紧跟字母
                        return false;
                    }else break; //后面是下个部分
                }
                word = std::to_string(x);
                i = j-1;
                int m = word.size();
                if(m>10) return false; // 长度不能超过10
                words.push_back(Word(word,NUMBER));
                continue;
            }
            if(is_delimiter(str[i])){ //分隔符
                word += str[i];
                words.push_back(Word(word,DELIMITER));
                continue;
            }
            if(is_operator(str[i])){ // 运算符
                word+=str[i];
                if(str[i]=='<' || str[i]=='>'){ // 处理<=  >=
                    if(i+1<n && str[i+1]=='=') word+=str[++i];
                }else if(str[i]==':'){
                    if(i+1==n) return false;
                    if(str[i+1]!='=') return false;
                    word+=str[++i];
                }
                words.push_back(Word(word,OPERATOR));
                continue;
            }
            if(isalpha(str[i])){ // 字母开头
                int j=i;
                for(;j<n;++j){
                    if(isalpha(str[j])){
                        if(islower(str[j])) word+=toupper(str[j]); // 小写转大写
                        else word+=str[j];
                    }else if(isdigit(str[j])){
                        word+=str[j];
                    }else break; //todo
                }
                i = j-1;
                //判断word是关键词还是标识符
                int m = word.size();
                if(m>10) return false;
                if(word=="CONST" || word=="VAR" || word=="PROCEDURE" || word=="BEGIN"
                || word=="END" || word=="ODD" || word=="IF" || word=="THEN" || word=="CALL"
                || word=="WHILE" || word=="DO" || word=="READ" || word=="WRITE"){
                    words.push_back(Word(word,KEYWORD));
                }else{
                    words.push_back(Word(word,IDENTIFIER));
                }
                continue;
            }
            return false; // 未知字符
        }

        return true; // 词法正确
    }
    void printWord() const {
        for(auto &word:words){
            word.print();
            std::cout << "\n";
        }
    }
};
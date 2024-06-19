#include <bits/stdc++.h>
#include "SyntaxAnalyzer.h"
using namespace std;
class Interpreter{
private:
    size_t PC, SP, BP;
    vector<Code> codes;
    vector<int>arr;
public:
    void init(){
        PC = SP = BP = 0;
        arr.clear();
        codes.clear();
        ifstream readfile("program.code");
        string s;
        while(getline(readfile, s, '\n')){
            istringstream in(s);
            string f;
            int l,a;
            in >> f >> l >> a;
            codes.emplace_back(f, l, a);
        }
    }

    int getLevel(size_t l){
        int level = BP;
        while(l){
            level = arr[level]; 
            l--;
        }
        return level;
    };

    void LIT(Code &code){
        arr.push_back(code.a), SP = arr.size()-1;
    }

    void LOD(Code &code){
        arr.push_back(arr[getLevel(code.l) + code.a]), SP = arr.size()-1;
    }

    void STO(Code &code){
        SP = arr.size()-1;
        arr[getLevel(code.l) + code.a] = arr[SP];
        arr.pop_back();
        SP = arr.size()-1;
    }

    void CAL(Code &code){
        arr.push_back(getLevel(code.l));
        arr.push_back(BP);
        arr.push_back(PC);
        BP = SP + 1;
        SP = arr.size()-1;
        PC = code.a;
    }

    void INT(Code &code){
        for(int i=0;i<code.a;i++) arr.push_back(0);
        SP = arr.size()-1;
    }

    void JMP(Code &code){
        PC = code.a;
    }

    void JPC(Code &code){
        SP = arr.size()-1;
        if(arr[SP] == 0){
            PC = code.a;
            arr.pop_back();
            SP = arr.size()-1;
        }
    }

    void OPR(Code &code){
        int x,y,z;
        x = y = z = 0;
        switch (code.a)
        {
            case 0:
                SP = arr.size()-1; 
                SP = BP-1;
                BP = arr[SP+2];
                PC = arr[SP+3];
                SP = arr.size()-1;
                break;
            case 1:
                SP = arr.size()-1;
                x = arr[SP-1];
                y = arr[SP];
                z = x + y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 2:
                SP = arr.size()-1;
                x = arr[SP-1];
                y = arr[SP];
                z = x-y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 3:
                SP = arr.size()-1;
                x = arr[SP];
                y = arr[SP-1];
                z = x*y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 4:
                SP = arr.size()-1;
                x = arr[SP-1];
                y = arr[SP];
                z = x/y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 5:
                SP = arr.size()-1;
                x = arr[SP]%2;
                arr.pop_back();
                arr.push_back(x);
                SP = arr.size()-1;
                break;
            case 6:
                SP = arr.size()-1;
                x = arr[SP-1];
                y = arr[SP];
                z = x==y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 7:
                SP = arr.size()-1;
                x = arr[SP];
                y = arr[SP-1];
                z = x != y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 8:
                SP = arr.size()-1;
                x = arr[SP-1];
                y = arr[SP];
                z = x<y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 9:
                SP = arr.size()-1;
                x = arr[SP-1];
                y = arr[SP];
                z = x <= y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 10:
                SP = arr.size()-1;
                x = arr[SP-1];
                y = arr[SP];
                z = x > y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 11:
                SP = arr.size()-1;
                x = arr[SP-1];
                y = arr[SP];
                z = x >= y;
                arr.pop_back();
                arr.pop_back();
                arr.push_back(z);
                SP = arr.size()-1;
                break;
            case 12:
                SP = arr.size()-1;
                cin >> x;
                arr.push_back(x);
                SP = arr.size()-1;	
                break;
            case 13:
                SP = arr.size()-1;
                cout << arr[SP] << "\n";
                arr.pop_back();
                SP = arr.size()-1;
                break;
            case 14:
                int ans = arr[SP];
                arr.pop_back();
                arr.push_back(-ans);
                SP = arr.size()-1;
                break;
        }
    }

    bool start(){
        init();
        while(PC + 1 != codes.size()){
            Code code = codes[PC++];
            if(code.f == "LIT") LIT(code);
            else if(code.f == "LOD") LOD(code);
            else if(code.f == "STO") STO(code);
            else if(code.f == "CAL") CAL(code);
            else if(code.f == "INT") INT(code);
            else if(code.f == "JMP") JMP(code);
            else if(code.f == "JPC") JPC(code);
            else if(code.f == "OPR") OPR(code);
            else return false;
        }
        return true;
    }
};

int main(){
    Interpreter in;
    if(!in.start()) return -1;
	return 0;
}
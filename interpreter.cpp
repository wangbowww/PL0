#include <bits/stdc++.h>
#include "SyntaxAnalyzer.h"
using namespace std;
vector<Code> codes; // 类汇编代码
size_t PC, SP, BP;
vector<int> arr;

int main(){
	ifstream readfile("program.code");
    string s;
	while(getline(readfile, s, '\n')){
		istringstream in(s);
        string f;
		int l,a;
		in >> f >> l >> a;
		codes.emplace_back(f, l, a);
	}
	PC = SP = BP = 0;
    auto getLevel = [&](size_t l){
        int level = BP;
        while(l){
            level = arr[level]; 
            l--;
        }
        return level;
    };
	while(PC != codes.size()-1){
		Code code = codes[PC++];
		if(code.f == "LIT") arr.push_back(code.a), SP = arr.size()-1;
		else if(code.f == "LOD") arr.push_back(arr[getLevel(code.l) + code.a]), SP = arr.size()-1;
		else if(code.f == "STO") {
			SP = arr.size()-1;
			arr[getLevel(code.l) + code.a] = arr[SP];
			arr.pop_back();
			SP = arr.size()-1;
		}else if(code.f == "CAL"){
			arr.push_back(getLevel(code.l));
			arr.push_back(BP);
			arr.push_back(PC);
            BP = SP + 1;
			SP = arr.size()-1;
			PC = code.a;
		}else if(code.f == "INT") {
			for(int i=0;i<code.a;i++) arr.push_back(0);
			SP = arr.size()-1;
		}else if(code.f == "JMP") PC = code.a;
		else if(code.f == "JPC") {
			SP = arr.size()-1;
			if(arr[SP] == 0){
				PC = code.a;
				arr.pop_back();
				SP = arr.size()-1;
			}
		}
		else if(code.f == "OPR"){
			if(code.a == 14){
				int ans = arr[SP];
				arr.pop_back();
				arr.push_back(-ans);
				SP = arr.size()-1;
			}
            else if(code.a == 0) {
				SP = arr.size()-1; 
				SP = BP-1;
				BP = arr[SP+2];
				PC = arr[SP+3];
				SP = arr.size()-1;
			}
			else if(code.a == 1) {
				SP = arr.size()-1;
				int x = arr[SP-1];
				int y = arr[SP];
				int z = x + y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			}
			else if(code.a == 2) {
				SP = arr.size()-1;
				int x = arr[SP-1];
				int y = arr[SP];
				int z = x-y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			}
			else if(code.a==3){
				SP = arr.size()-1;
				int x = arr[SP];
				int y = arr[SP-1];
				int z = x*y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			}
			else if(code.a == 4) {
				SP = arr.size()-1;
				int x = arr[SP-1];
				int y = arr[SP];
				int z = x/y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			}
			else if(code.a == 5) {
				SP = arr.size()-1;
				int x = arr[SP]%2;
				arr.pop_back();
				arr.push_back(x);
				SP = arr.size()-1;
			}
			else if(code.a == 6) {
				SP = arr.size()-1;
				int x = arr[SP-1];
				int y = arr[SP];
				int z = x==y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			}
			else if(code.a == 7) {
				SP = arr.size()-1;
				int x = arr[SP];
				int y = arr[SP-1];
				int z = x != y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			}
			else if(code.a == 8){
				SP = arr.size()-1;
				int x = arr[SP-1];
				int y = arr[SP];
				int z = x<y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			} 
			else if(code.a == 9){
				SP = arr.size()-1;
				int x = arr[SP-1];
				int y = arr[SP];
				int z = x <= y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			} 
			else if(code.a == 10){
				SP = arr.size()-1;
				int x = arr[SP-1];
				int y = arr[SP];
				int z = x > y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			} 
			else if(code.a == 11) {
				SP = arr.size()-1;
				int x = arr[SP-1];
				int y = arr[SP];
				int z = x >= y;
				arr.pop_back();
				arr.pop_back();
				arr.push_back(z);
				SP = arr.size()-1;
			}
			else if(code.a == 12) {
				SP = arr.size()-1;
				int x;
				cin >> x;
				arr.push_back(x);
				SP = arr.size()-1;	
			}
			else if(code.a == 13) {
				SP = arr.size()-1;
				cout << arr[SP] << "\n";
				arr.pop_back();
				SP = arr.size()-1;
			}
		}
		else {
			return -1;
		}
	}
	return 0;
}
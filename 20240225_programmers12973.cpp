#include <iostream>
#include<string>
#include <stack>
using namespace std;

stack<char> stringStack;

bool Destroy(string s){
    int stringSize = s.size();
    for(int i=0;i<stringSize;i++){
        char c = s[i];
        if(stringStack.size()>0){
            if(stringStack.top()==c) stringStack.pop();//넣지말고 제거
            else stringStack.push(c);
        }else stringStack.push(c);
    }
    if(stringStack.empty()) return true;
    return false;
}
int solution(string s)
{
    return Destroy(s);
}

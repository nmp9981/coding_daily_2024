#include <iostream>
#include <stack>
using namespace std;

int n;

//스택 원소 이동
void MoveStackElement(string str) {
    //스택 원소 이동
    stack<char> waitStack;
    for (int i = 0; i < str.length(); i++) {
        char curStr = str[i];

        if(waitStack.empty()){
            waitStack.push(curStr);
        }else{
            if(curStr==')' && waitStack.top()=='('){
                waitStack.pop();
            }else{
                waitStack.push(curStr);
            }
        }
    }
    //결과 확인
    if (waitStack.empty())
        cout << "YES"<<"\n";
    else cout << "NO\n";
}

//입력
void Input() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    for (int i = 0; i < n; i++) {
        string s;
        cin>> s;
        MoveStackElement(s);
    }
}


int main()
{
    Input();
    return 0;
}

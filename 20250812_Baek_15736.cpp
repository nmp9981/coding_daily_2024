#include <iostream>
using namespace std;

int n;

//제곱수 개수 구하기
void CountSquare(){
    int cnt = 0;
    int num = 1;
    while(num*num<=n){
        cnt++;
        num++;
    }
    cout<<cnt;
}

//입력
void Input(){
    cin>>n;
}

int main()
{
    Input();//입력
    CountSquare();//제곱수 구하기
    return 0;
}

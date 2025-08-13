#include <iostream>
using namespace std;

int n;

void CountSquare(){
    int cnt = 0;
    int num = 1;
    while(num*num<=n){
        cnt++;
        num++;
    }
    cout<<cnt;
}

int main()
{
    cin>>n;
    CountSquare();
    return 0;
}

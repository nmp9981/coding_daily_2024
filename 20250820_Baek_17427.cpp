#include <iostream>
using namespace std;

int n;

//약수의 합 구하기
void SumFactor(){
    long long sumFactor = 0;
    for(int i=1;i<=n;i++){
        sumFactor += i*(n/i);
    }
    cout<<sumFactor;//출력
}

//입력
void Input(){
    cin>>n;
}

int main()
{
    Input();//입력
    SumFactor();//약수의 합
    return 0;
}

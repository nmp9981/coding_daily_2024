#include <iostream>
using namespace std;

int n;
const int mod = 1000000007;
const int maxDigitSum = 7575;
const int maxN = 1516;
int countMul15DP[maxN][maxDigitSum];//자릿수, 총합

//15의 배수 개수 세기
void CountMul15(){
    countMul15DP[1][1] = 1;
    countMul15DP[1][5] = 1;
    countMul15DP[2][2] = 1;
    countMul15DP[3][3] = 1;
    countMul15DP[4][4] = 1;
    countMul15DP[5][5] = 1;

    for(int i=2;i<maxN;i++){
        for(int k=6;k<maxDigitSum;k++){
            countMul15DP[i][k] =(countMul15DP[i-1][k-1]+countMul15DP[i-1][k-5])%mod;
        }
    }
}

//3의 배수 검사
void InspectMul3(){
    int count15 = 0;//15배수의 개수
    //합이 3n-5꼴 : 일의자리는 반드시 5
    for(int k=1;k<maxDigitSum;k+=3){//k는 총합
        count15+=countMul15DP[n-1][k];//n-1자리
        count15%=mod;
    }
    cout<<count15;
}

//입력
void Input(){
    cin>>n;
}

int main()
{
    CountMul15();//15의 배수 개수 세기
    Input();//입력
    InspectMul3();//3의 배수 검사
    return 0;
}

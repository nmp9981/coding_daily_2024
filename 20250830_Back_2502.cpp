#include <iostream>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

const int maxi = 31;
int pibo[maxi];
int d,k;

//피보나치 수
void Fibo(){
    pibo[1] = 1;
    pibo[2] = 1;
    for(int i=3;i<maxi;i++){
        pibo[i] = pibo[i-1]+pibo[i-2];
    }
}

//떡먹기
void EatRicecake(){
    int maxA = k/pibo[d-2];
    for(int a=1;a<=maxA;a++){
        int det = k-pibo[d-2]*a;
        if(det%pibo[d-1]==0){
            cout<<a<<"\n";
            cout<<det/pibo[d-1];
            break;
        }
    }
}

//입력
void Input(){
    cin>>d>>k;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    Fibo();//피보나치 수
    Input();//입력
    EatRicecake();//떡먹기
    return 0;
}

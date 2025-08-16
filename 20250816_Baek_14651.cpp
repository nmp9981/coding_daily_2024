#include <iostream>
using namespace std;
typedef long long ll;

int n;
const int mod = 1000000009;

//3의 배수 개수 세기 : 2*3^(n-2)
ll CountMulti3(){
    if(n==1) return 0;//예외
    
    ll cnt = 1;
    for(int i=0;i<n-2;i++){
        cnt = (cnt*3)%mod;
    }
    cnt = (2*cnt)%mod;
    return cnt;
}

//입력
void Input(){
    cin>>n;
}

int main()
{
    Input();//입력
    cout<<CountMulti3();//3의 배수의 개수 세기
    return 0;
}

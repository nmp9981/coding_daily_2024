#include <iostream>
#include <vector>
#include <cstring>
using namespace std;
typedef long long ll;

ll a,b,c;

//곱셈
ll Mul(ll x,ll y, ll mod){
    if(y==1) return x%mod;
    
    ll p = Mul(x,y/2,mod)%mod;
    
    if(y%2==1){
        return (((p*p)%mod)*x)%mod;
    }else{
        return (p*p)%mod;
    }
}

//입력
void Input(){
    cin>>a>>b>>c;
    cout<<Mul(a,b,c);
}

int main()
{
    Input();//입력
    return 0;
}

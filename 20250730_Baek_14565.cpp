#include <iostream>
using namespace std;
typedef long long ll;

ll n,a;

//cx = 1mod(k)
//확장 유클리드 알고리즘
ll ExpandEuclid(ll a,ll b, ll &x, ll &y){
    if(b==0) {
        x=1;
        y=0;
        return a;
    }
    
    ll x1,y1;
    ll gcd = ExpandEuclid(b, a%b,x1,y1);
    x = y1;
    y = x1-(a/b)*y1;
    return gcd;
}

//입력
void Input(){
    cin>>n>>a;
    //덧셈역은 n-a
    
    //곱셉역
    ll x,y;//y가 곱셈 역원
    ll gcd_Value = ExpandEuclid(n,a,x,y);
    if(gcd_Value!=1){
        cout<<n-a<<" "<<-1;
    }else{
        y=(y%n+n)%n;//양수화
        cout<<n-a<<" "<<y;
    }
}

int main()
{
    Input();
    return 0;
}

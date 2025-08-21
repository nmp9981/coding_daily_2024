#include <iostream>
using namespace std;
typedef long long ll;

ll n,k;
const ll mod = 1000000007;
const int maxi = 1002;
ll pascal[maxi][maxi];//파스칼의 삼각형
ll SumExponent[maxi];//멱수의 합

// 모듈러 곱셈
ll Mul(ll a, ll b) {
    return (a * b) % mod;
}

// 모듈러 거듭제곱 (a^p % MOD)
ll power(ll a, ll p) {
    ll res = 1;
    a %= mod;
    while (p > 0) {
        if (p % 2 == 1) {
            res = Mul(res, a);
        }
        a = Mul(a, a);
        p /= 2;
    }
    return res;
}

// 모듈러 역원 (a의 MOD-2승)
ll ModInverse(ll n) {
    return power(n, mod - 2);
}

//파스칼의 삼각형
void PascalTriangle(){
    pascal[1][0] = 1;
    pascal[1][1] = 1;
    
    for(int i=2;i<maxi;i++){
        for(int j=0;j<=i;j++){
            if(j==0 || j==i) pascal[i][j] = 1;
            else pascal[i][j] = (pascal[i-1][j]+pascal[i-1][j-1])%mod;
        }
    }
}

//멱수의 합 계산
void CalSumExponent(){
    SumExponent[0] = n;//초기 값
    for(int i=1;i<=k;i++){
        //(n+1)^(k+1)-1
        ll term1 = (power(n + 1, i + 1) - 1 + mod) % mod;
        
        //이항계수 활용, sum(C(n,r)*s(i))
        ll sum_terms = 0;
        for (int j = 0; j < i; j++) {
            sum_terms = (sum_terms + Mul(pascal[i + 1][j], SumExponent[j])) % mod;
        }
        
        ll term2 = (term1 - sum_terms + mod) % mod;
        ll inv = ModInverse(i + 1);///i+1 모듈러 역원
        SumExponent[i] = Mul(term2, inv);
    }
    cout<<SumExponent[k];//출력
}

//입력
void Input(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin>>n>>k;
}

int main()
{
    PascalTriangle();//파스칼의 삼각형
    Input();//입력
    CalSumExponent();//멱수의 합 계산
    return 0;
}

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;
typedef unsigned long long ll;

vector<ll> stake;

/// <summary>
/// 기능 ; 두 수의 최대공약수 구하기
/// </summary>
ll GCD(ll x, ll y)
{
    if (x < y) swap(x, y);
    while (y != 0)
    {
        ll r = x % y;
        x = y;
        y = r;
    }
    return x;
}

int main() {
	//빠른 입력
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	
    int t;
    cin>>t;
    while(t--){
        ll n;
        cin>>n;
        stake.push_back(n);
    }
    
    ll lcd = 1;
    for(int i=0;i<stake.size();i++){
        int gcd = GCD(stake[i],lcd);
        lcd = (lcd*stake[i])/gcd;
    }
    cout<<2*lcd;
	return 0;
}

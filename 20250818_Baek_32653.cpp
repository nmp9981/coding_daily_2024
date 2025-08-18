#include <iostream>
#include <vector>
using namespace std;
typedef long long ll;

int n;
vector<ll> stake;


//GCD
ll GCD(ll x, ll y){
    if (x < y) swap(x, y);
    while (y != 0)
    {
        ll r = x % y;
        x = y;
        y = r;
    }
    return x;
}

int main()
{
    cin>>n;
    for(int i=0;i<n;i++){
        ll x;
        cin>>x;
        stake.push_back(x);
    }

    ll lcd = 1;
    for(int i=0;i<n;i++){
        ll gcd = GCD(lcd,stake[i]);
        lcd = (lcd*stake[i])/gcd;
    }
    cout<<2*lcd;
    return 0;
}

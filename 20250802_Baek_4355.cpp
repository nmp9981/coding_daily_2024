#include <iostream>
using namespace std;
typedef long long ll;

ll n;

//오일러 피 함수
ll Euler(ll n){
    ll res = n;
    for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) res = res-(res/i);
		while (n % i == 0) n = n/i;
	}

	if (n > 1) res = res-(res/n);
	return res;
}

//입력
void Input(){
    ios::sync_with_stdio(0);
	cin.tie(0);
	
	cin>>n;
	while(n){
	    if(n==1) cout<<0<<"\n";    
	    else{
	        cout<<Euler(n)<<"\n";
	    }
	    cin>>n;
	}
}

int main()
{
    Input();//입력
    return 0;
}

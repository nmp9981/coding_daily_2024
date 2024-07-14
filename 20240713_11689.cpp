#include <iostream>
using namespace std;
typedef long long ll;

ll n, res;

ll Euler(ll n){
    res = n;
    for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) res = res-(res/i);
		while (n % i == 0) n = n/i;
	}

	if (n > 1) res = res-(res/n);
	return res;
}
int main()
{
	cin >> n;
	cout<<Euler(n);
	return 0;
}

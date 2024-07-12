#include <iostream>
using namespace std;
typedef long long ll;

ll n, ans;

ll Euler(ll n){
    ans = n;
    for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0) ans -= (ans / i);
		while (n % i == 0) n /= i;
	}

	if (n > 1) ans -= (ans / n);
	return ans;
}

int main()
{
	cin >> n;
	cout<<Euler(n);
	return 0;
}

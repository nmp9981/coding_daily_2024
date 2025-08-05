#include <iostream>
using namespace std;

int n, test;
int main()
{
    ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> n;
	while (n) {
		int ans = 0;
		for (int i = 5; i <= n; i *= 5) ans += n / i;
		cout << "Case #" << ++test << ": " << ans << '\n';
		cin >> n;
	}

    return 0;
}

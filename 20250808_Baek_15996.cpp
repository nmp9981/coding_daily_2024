#include <iostream>
using namespace std;
typedef long long ll;

ll n, a;

//입력
void Input() {
    cin >> n >> a;
}

//팩토리얼 나누기
void FactDiv() {
    ll k = 0;
    for (ll i = a; i <= n; i *= a) {
        k += (n / i);
    }
    cout << k;
}

int main()
{
    Input();
    FactDiv();
    return 0;
}

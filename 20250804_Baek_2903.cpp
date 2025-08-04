#include <iostream>
using namespace std;

int n;

//입력
void Input() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    int ans = 1;
    for (int i = 0; i < n; i++) ans *= 2;

    ans += 1;
    cout << ans * ans;
}

int main()
{
    Input();
    return 0;
}

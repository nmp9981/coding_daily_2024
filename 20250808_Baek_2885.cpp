#include <iostream>
#include <string>
using namespace std;

int n;

//입력
void Input() {
    cin >> n ;
}

//초콜릿 크기
int ChocoSize() {
    int size = 1;
    while (size < n) {
        size *= 2;
    }
    return size;
}

//초콜릿 크기
int ChocoDiv(int size) {
    if (size == n) return 0;

    int cnt = 0;
    string bin = "0";
    bool isDiv = false;

    while (n>0) {
        //초콜릿 나누기 시작
        if (n % 2 != 0 && !isDiv) {
            isDiv = true;
        }
        if (isDiv) cnt++;

        bin += to_string(n%2);
        n = n / 2;
    }
    return cnt;
}

int main()
{
    Input();

    int _chocoSize = ChocoSize();
    int _chocoDiv = ChocoDiv(_chocoSize);
    cout << _chocoSize << " " << _chocoDiv;
    return 0;
}

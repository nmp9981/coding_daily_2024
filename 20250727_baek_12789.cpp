#include <iostream>
#include <stack>
using namespace std;

int n;
const int maxi = 1001;
stack<int> waitStack;
int lineNumberList[maxi];

//입력
void Input() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> n;
    for (int i = 0; i < n; i++) {
        cin >> lineNumberList[i];
    }
}

//스택 원소 이동
void MoveStackElement() {
    int exitNum = 1;//나가야하는 번호
    //스택 원소 이동
    for (int i = 0; i < n; i++) {
        int curNum = lineNumberList[i];

        waitStack.push(curNum);
        if (!waitStack.empty()) {
            while (exitNum == waitStack.top()) {
                waitStack.pop();
                exitNum += 1;

                //stack이 빌 경우
                if (waitStack.empty())
                    break;
            }
        }
    }
    //결과 확인
    if (waitStack.size() == 0)
        cout << "Nice";
    else cout << "Sad";
}

int main()
{
    Input();
    MoveStackElement();
    return 0;
}

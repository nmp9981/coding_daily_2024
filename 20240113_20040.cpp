#include <iostream>
using namespace std;

int n,m;
const int maxi = 500001;
int root[maxi];//각 노드의 루트노드

//초기화
void Init() {
    for (int i = 0; i < maxi; i++) root[i] = i;
}
//루트노드 찾기
int Find(int x) {
    if (root[x] == x) return x;
    return root[x] = Find(root[x]);
}
//병합
void Merge(int fx, int fy) {
    int rx = Find(fx);
    int ry = Find(fy);
    if (rx == ry) return;
    else root[rx] = ry;
}
//입력
void Input() {
    cin >> n >> m;//입력
    for (int i = 1; i <= m; i++) {
        int a, b;
        cin >> a >> b;
        if (Find(a) == Find(b)) {//루트노드가 같으면 사이클
            cout << i;
            return;
        }
        Merge(a, b);
    }
    cout << 0;//사이클이 없다.
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    Init();//초기화
    Input();//입력
    return 0;
}

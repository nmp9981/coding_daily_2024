#include <iostream>
using namespace std;

int n;

//오일러 피함수
int EulerPI(int n){
    int res = n;
    for (int i = 2; i * i <= n; i++) {
		if (n % i == 0) res = res-(res/i);
		while (n % i == 0) n = n/i;
	}

	if (n > 1) res = res-(res/n);
	return res;
}

//간선 개수 세기
void CountEdge(){
    int cnt = 0;
    for(int i=2;i<=n;i++){
        cnt+=EulerPI(i);
    }
    cout<<cnt;
}

//입력
void Input(){
    cin>>n;
}

int main()
{
    Input();//입력
    CountEdge();//간선 개수 세기
    return 0;
}

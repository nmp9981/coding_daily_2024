#include <iostream>
using namespace std;

int n;
int countEuler[10001];

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

//간선 개수 구하기
void EdgeCount(){
    for(int k=1;k<=10000;k++){
        countEuler[k] = countEuler[k-1]+EulerPI(k);   
    }
}

//입력
void Input(){
    int test;
    cin>>test;
    
    while(test--){
        int n;
        cin>>n;
        //0/n 포함
        cout<<countEuler[n]+1<<"\n";
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    EdgeCount();//오일러 피함수 총합
    Input();//입력
    return 0;
}

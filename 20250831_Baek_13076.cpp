#include <iostream>
using namespace std;

const int maxi = 10001;
//i까지의 누적 오일러 피함수값(누적 서로소 개수)
int countEulerPI[maxi];

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

//누적 오일러 피함수값 구하기
void AccumulateEulerPI(){
    for(int i=1;i<maxi;i++){
        countEulerPI[i] = countEulerPI[i-1]+EulerPI(i);
    }
}

//입력
void Input(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    int test;
    cin>>test;
    while(test--){
        int n;cin>>n;
        cout<<countEulerPI[n]+1<<"\n";
    }
}

int main()
{
    AccumulateEulerPI();//누적 오일러 피함수
    Input();//입력
    return 0;
}

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

int n;
vector<int> factorList;

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

//N의 약수 구하기
void FactorOfN(int n){
    int rootN = sqrt(n);
    for(int i=1;i<=rootN;i++){
        //n의 약수(제수, 몫 둘다 넣는다.)
        if(n%i==0){
            factorList.push_back(i);
            factorList.push_back(n/i);
        }
    }
    sort(factorList.begin(),factorList.end());//오름차순 정렬
}

//방정식 풀이
void SolveEquation(){
    int factorSize = factorList.size();
    
    for(int i=0;i<factorSize;i++){
        int x = factorList[i];
        int eulerValue = EulerPI(x);
        
        //해를 찾음
        if(x*eulerValue==n){
            cout<<x;
            return;
        }
    }
    //해가 없음
    cout<<-1;
}

//입력
void Input(){
    cin>>n;
}

int main()
{
    Input();//입력
    FactorOfN(n);//N의 약수 구하기
    SolveEquation();//방정식 풀이
    return 0;
}

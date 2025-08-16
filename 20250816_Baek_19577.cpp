#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
using namespace std;

int k,n, res;
vector<int> factorList;

//오일러 피함수
int EulerPI(int n){
    res = n;
    for (int i = 2; i * i <= n; i++) {
		if (n % i == 0) res = res-(res/i);
		while (n % i == 0) n = n/i;
	}

	if (n > 1) res = res-(res/n);
	return res;
}

//약수 구하기
void Factor(int n){
     int sqrtN = sqrt(n);
    for(int i=1;i<=sqrtN;i++){
        if(n%i==0){
            factorList.push_back(i);
            factorList.push_back(n/i);
        }
    }
    //오름차순 정렬
    sort(factorList.begin(),factorList.end());
}

//방정식 풀이
void SolveEquation(){
    int factorSize = factorList.size();
    
    for(int i=0;i<factorSize;i++){
        int factor = factorList[i];
        int eulerFactor = EulerPI(factor);
        
        if(factor*eulerFactor == k){
            cout<<factor;
            return;
        }
    }
    cout<<-1;
}

int main()
{
	cin >> k;
	Factor(k);
	SolveEquation();
	return 0;
}

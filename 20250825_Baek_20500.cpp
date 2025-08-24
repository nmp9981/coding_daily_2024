#include <iostream>
using namespace std;

int n;
const int maxi = 1516;
const int sumMaxi = 7576;
const int mod = 1000000007;
//(자릿수, 총합) 개수
int countDigitSumDP[maxi][sumMaxi];

//i자릿수, 합 j일때의 개수
void CountDigitSum(){
    //초기값
    countDigitSumDP[1][1] = 1;
    countDigitSumDP[1][5] = 1;
    countDigitSumDP[2][2] = 1;
    countDigitSumDP[3][3] = 1;
    countDigitSumDP[4][4] = 1;
    countDigitSumDP[5][5] = 1;
    
    //개수 구하기
    for(int i=2;i<maxi;i++){
        for(int j=6;j<sumMaxi;j++){
            countDigitSumDP[i][j] = (countDigitSumDP[i-1][j-1]+countDigitSumDP[i-1][j-5])%mod;
        }
    }
}

//15의 배수 세기
void Count15Mul(){
    int cnt = 0;
    //맨뒤 5는 확정이므로 n-1자리수만 세준다.
    //합이 3k-5를 만족해야한다.(k>=2인 정수)
    for(int sum=1;sum<sumMaxi;sum+=3){
        cnt+=countDigitSumDP[n-1][sum];
        cnt%=mod;
    }
    cout<<cnt;//출력
}

//입력
void Input(){
    cin>>n;
}

int main()
{
    CountDigitSum();//i자릿수, 합 j일때의 개수
    Input();//입력
    Count15Mul();//15의 배수 개수 세기
    return 0;
}

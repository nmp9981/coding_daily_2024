#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
using namespace std;

int k;
const int primeMaxRange = 1000000;//최대 소수 범위 
vector<int> primeList;//소수 리스트
bool isPrime[1000000];//1000000까지의 소수 여부
vector<int> factorizationPrimesList;//소인수 리스트

//에라토스테네스의 체
void Eratosthenes(){
    //에라토스테네스의 체
    primeList.push_back(2);//2도 소수
    memset(isPrime,true,sizeof(isPrime));//true로 초기화

    int rootPrimeMaxRange = 1000;//root 1000000
    for(int i=3;i<rootPrimeMaxRange;i++){
        if(isPrime[i]){//소수이면
            for(int j=i*i;j<primeMaxRange;j+=i) isPrime[j] = false;//소수가 아님
        }
    }
    //소수 등록
    for(int i=3;i<primeMaxRange;i+=2){
        if(isPrime[i]) primeList.push_back(i);
    }
}

//소인수분해
void FactorizationIntoPrimes(){
    int idx = 0;
    while(k>1){
        if(k%primeList[idx]==0){//나눌 수 있으면 계속 나눈다.
            k/=primeList[idx];
            factorizationPrimesList.push_back(primeList[idx]);
        }else idx+=1;//다음 소수
    } 
}

//입력
void Input(){
    cin>>k;
}

//출력
void Output(){
    int countfactorizationPrimes = factorizationPrimesList.size();
    cout<<ceil(log2(countfactorizationPrimes));//완전 이진트리의 깊이
}

int main() {
    //빠른 입력
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    Eratosthenes();//에토스테네스의 체
    Input();//입력
    FactorizationIntoPrimes();//소인수분해
    Output();//출력
    return 0;
}

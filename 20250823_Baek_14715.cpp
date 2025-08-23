#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
using namespace std;

int n;
const int maxi = 1000001;
bool isPrime[maxi];//소수 여부
vector<int> primeList;//소수 리스트
vector<int> factorizationPrimesList;//소인수 리스트

//에라토스테네스의 체
void Eratosthenes(){
    //초기화
    int rootMaxi = 1000;
    primeList.push_back(2);//2도 소수
    memset(isPrime,true,sizeof(isPrime));//초기화
    
    //에라토스테네스의 체
    for(int i=3;i<rootMaxi;i+=2){
        if(isPrime[i]){//소수라면
            for(int j=i*i;j<maxi;j+=i) isPrime[j] = false;
        }
    }
    
    //소수 등록
    for(int i=3;i<maxi;i+=2){
        if(isPrime[i]) primeList.push_back(i);
    }
}

//소인수분해
void FactorizationIntoPrimes(){
    int idx = 0;
    while(n>1){
        if(n%primeList[idx]==0){//소인수
            n/=primeList[idx];
            factorizationPrimesList.push_back(primeList[idx]);
        }else idx+=1;//다음 소수
    }
}

//입력
void Input(){
    cin>>n;
}

//출력
void Output(){
    int depth = ceil(log2(factorizationPrimesList.size()));//완전이진트리 깊이
    cout<<depth;//출력
}

int main()
{
    Eratosthenes();//에라토스테네스의 체
    Input();//입력
    FactorizationIntoPrimes();//소인수분해
    Output();//출력
    return 0;
}

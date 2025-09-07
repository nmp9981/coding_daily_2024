#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

int a,b;
int primeListSize;
const int maxi = 99999;
bool isPrime[maxi];//소수 여부
vector<int> primeList;//소수 목록

//에라토스테네스의 체
void Eratostenes(){
    int rootMaxi = 316;
    memset(isPrime, true, sizeof(isPrime));//초기화
    
    //에라토스테네스의 체
    for(int i=2;i<rootMaxi;i++){
        if(isPrime[i]){//소수이면
            for(int j=i*i;j<maxi;j+=i) isPrime[j] = false;//소수가 아님
        }
    }
    
    //소수 목록
    for(int i=2;i<maxi;i++){
        if(isPrime[i]) primeList.push_back(i);
    }
    primeListSize = primeList.size();
}

//소인수 길이
int Size_FactorizationIntoPrimes(int k){
    int primeIdx = 0;//2부터 분해 시도
    vector<int> primeFactorList;//소인수 리스트
        
    while(k>1){
        if(k%primeList[primeIdx]==0){//소인수
            k/=primeList[primeIdx];
            primeFactorList.push_back(primeList[primeIdx]);
        }else primeIdx++;//다음 소인수
    }
    return primeFactorList.size();
}

//언더프라임
void UnderPrime(){
    int underPrimeCount = 0;//언더프라임 개수
    for(int i=a;i<=b;i++){
        if(isPrime[i]) continue;//소수면 언더프라임이 아니다.
        
        //언더프라임 판정
        int primeFactorListSize = Size_FactorizationIntoPrimes(i);
        for(int j=0;j<primeListSize;j++){
            if(primeList[j]>16) continue;//100000범위에서는 이 이상의 소인수 개수가 나오지 않는다.
            
            if(primeFactorListSize==primeList[j]){
                underPrimeCount++;
                break;
            }
        }
    }
    cout<<underPrimeCount;//출력
}

//입력
void Input(){
    cin>>a>>b;
}

int main()
{
    Eratostenes();//에라토스테네스의 체
    Input();//입력
    UnderPrime();//언더프라임
    return 0;
}

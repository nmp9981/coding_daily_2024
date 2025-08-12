#include <iostream>
#include <vector>
#include <cstring>
#include <cmath>
using namespace std;
typedef long long ll;

ll a,b;
int maxPrimeRange = 10000000;
int maxPow = 47;
bool isPrime[10000000];//소수 여부
vector<int> primeList;//소수 목록

//에라토스테네스의 체
void EratosTenes(){
    int sqrtMaxPrimeRange = 3163;//root(10^7)
    primeList.push_back(2);//2도 소수
    
    memset(isPrime,true,sizeof(isPrime));//true로 초기화
    for(int i=3;i<sqrtMaxPrimeRange;i+=2){
        if(isPrime[i]){//소수라면
            for(int j=i*i;j<maxPrimeRange;j+=i){
                isPrime[j] = false;//소수가 아님
            }    
        }
    }
    
    //소수 등록
    for(int i=3;i<maxPrimeRange;i+=2){
        if(isPrime[i]) primeList.push_back(i);
    }
}

//거의 소수 개수 구하기
int CountAlmostPrime(ll a, ll b){
    int cnt = 0;
    int primeSize = primeList.size();
    
    for(int n=2;n<maxPow;n++){//N제곱
        for(int i=0;i<primeSize;i++){//소수 순회
            ll value = pow(primeList[i],n);
            if(value > b) break;//k보다 크면 종료
            
            if(value >= a) cnt++;//a이상일 경우만 개수를 센다
        }
    }
    return cnt;
}

//입력
void Input(){
    cin>>a>>b;
}

//출력
void Output(){
    int ans = CountAlmostPrime(a,b);//a~b거의 소수 개수
    cout<<ans;
}

int main()
{
    Input();//입력
    EratosTenes();//에라토스테네스의 체
    Output();//출력
    return 0;
}

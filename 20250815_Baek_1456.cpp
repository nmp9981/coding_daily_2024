#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>
using namespace std;
typedef long long ll;

ll a,b;
const int pow_max = 47;
const int primeMaxRange = 10000000;//최대 소수 범위 
vector<int> primeList;//소수 리스트
bool isPrime[10000000];//10000000까지의 소수 여부

//에라토스테네스의 체
void Eratosthenes(){
    //에라토스테네스의 체
    primeList.push_back(2);//2도 소수
    memset(isPrime,true,sizeof(isPrime));//true로 초기화
    
    int rootPrimeMaxRange = 3163;//root 10000000
    for(int i=3;i<rootPrimeMaxRange;i+=2){
        if(isPrime[i]){//소수면
            for(int j=i*i;j<primeMaxRange;j+=i) isPrime[j] = false;
        }
    }
    
    //소수 등록
    for(int i=3;i<primeMaxRange;i+=2){
        if(isPrime[i]) primeList.push_back(i);
    }
}

//거의 소수 개수 세기
void CountAlmostPrime(){
    int cnt = 0;
    int primeSize = primeList.size();
    
    for(int n=2;n<pow_max;n++){//소수의 n제곱
        for(int i=0;i<primeSize;i++){
            int curPrime = primeList[i];//현재 검사할 소수
            ll num = pow(curPrime,n);
            
            if(num>b) break;//범위 초과
            if(num>= a && num<=b) cnt++;//거의 소수
        }
    }
    cout<<cnt;
}

//입력
void Input(){
    cin>>a>>b;
}

int main()
{
    Eratosthenes();
    Input();//입력
    CountAlmostPrime();//거의 소수 개수 세기
    return 0;
}

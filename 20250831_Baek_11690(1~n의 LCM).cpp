#include <iostream>
#include <cstring>
#include <vector>
using namespace std;
typedef unsigned long long ull;

ull n;
const ull mod = 4294967296;
const int maxi = 100000000;
bool isPrime[maxi];
vector<int> primeList;//소수 목록 
int primeListSize;//소수 개수


//에라토스테네스의 체
void Eratosthenes(){
    //초기화
    int rootMaxi = 10000;
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
    primeListSize = primeList.size();//소수 총 개수
}

//거듭제곱 (a^m)
ull Power(ull a,ull m){
    if(m==1) return a;
    
    ull powerHalf = Power(a,m/2)%mod;
    if(m%2==0){
        return (powerHalf*powerHalf)%mod;
    }else{
        return (((powerHalf*powerHalf)%mod)*a)%mod;
    }
}

//최소공배수
void LCM(){
    ull lcm = 1;
    //n이 각 소수별로 몇번 나눠지는지 구한다.
    for(int i=0;i<primeListSize;i++){
        //n보다 큰 소수면 중단
        if(n<primeList[i]) break;
        
        int cnt = 0;
        int k = n;
        while(k>=primeList[i]){
            k/=primeList[i];
            cnt+=1;
        }
        
        //곱셈
        lcm = (lcm*Power(primeList[i],cnt))%mod;
    }
    cout<<lcm;
}

//입력
void Input(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin>>n;
}

int main()
{
    Eratosthenes();//에라토스테네스의 체로 소수를 미리 구한다.
    Input();//입력
    LCM();//최소공배수
    return 0;
}

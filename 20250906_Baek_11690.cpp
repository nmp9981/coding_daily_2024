#include <iostream>
#include <cstring>
#include <vector>
using namespace std;
typedef unsigned long long ull;

int n;
int primeSize;
const ull mod = 4294967296;
const int maxi = 99999999;
bool isPrime[maxi];
vector<int> primeList;

//에라토스테네스의 체
void Eratostenes(){
    const int rootMaxi = 9999;
    memset(isPrime,true,sizeof(isPrime));//초기화
    primeList.push_back(2);//2도 소수
    
    //에라토스테네스의 체
    for(int i=3;i<rootMaxi;i+=2){
        if(isPrime[i]){//i가 소수일때
            for(int j=i*i;j<maxi;j+=i) isPrime[j] = false;//j는 소수가 아님
        }
    }
    
    //소수 등록
    for(int i=3;i<maxi;i+=2){
        if(isPrime[i]) primeList.push_back(i);
    }
    primeSize = primeList.size();
}

//거듭제곱
ull Power(ull x, int a){
    if(a==1) return x%mod;
    
    ull halfPowerX = Power(x,a/2)%mod;
    
    if(a%2==0) return (halfPowerX*halfPowerX)%mod;
    else return (((halfPowerX*halfPowerX)%mod)*x)%mod;
}

//1~n의 최소공배수 구하기
void LCM_1toN(){
    ull lcm = 1;
   
    for(int i=0;i<primeSize;i+=1){
        int k = n;
        int count = 0;
        //입력 수보다 더 큰수로 나눌 수 없음
        if(primeList[i]>n) break;
        
        ull divNum = primeList[i];
        while(k>=divNum){
            k/=divNum;
            count+=1;
        }
        
        lcm = (lcm*Power(divNum,count))%mod;
    }
    cout<<lcm;
}

//입력
void Input(){
    cin>>n;
}

int main()
{
    Eratostenes();//에라토스테네스의 체
    Input();//입력
    LCM_1toN();//최소공배수
    return 0;
}

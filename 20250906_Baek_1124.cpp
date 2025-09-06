#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

int a,b;
int primeSize;
const int maxi = 99999;
bool isPrime[maxi];
vector<int> primeList;

//에라토스테네스의 체
void Eratostenes(){
    const int rootMaxi = 316;
    memset(isPrime,true,sizeof(isPrime));//초기화
    
    //에라토스테네스의 체
    for(int i=2;i<rootMaxi;i++){
        if(isPrime[i]){//i가 소수일때
            for(int j=i*i;j<maxi;j+=i) isPrime[j] = false;//j는 소수가 아님
        }
    }
    
    //소수 등록
    for(int i=2;i<maxi;i++){
        if(isPrime[i]) primeList.push_back(i);
    }
    primeSize = primeList.size();
}

//언더프라임
void UnderPrime(){
    int cnt = 0;
    for(int i=a;i<=b;i++){
        if(isPrime[i]) continue;//소수 (언더프라임 X)
    
        //소인수분해
        int curNum = i;
        vector<int> primeFactorList;
        for(int k=0;k<primeSize;k++){
            if(primeList[k]>curNum) break;
            
            //나눠떨어지면 소인수
            while(curNum%primeList[k]==0){
                primeFactorList.push_back(primeList[k]);
                curNum/=primeList[k];
            }
        }
        
        //언더프라임 검사
        int primeFactorListSize = primeFactorList.size();
        for(int k=0;k<primeSize;k++){
            if(primeList[k]>17) break;//2^17이상일 수 없다
            
            if(primeList[k]==primeFactorListSize){
                cnt+=1;
                break;
            }
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
    Eratostenes();//에라토스테네스의 체
    Input();//입력
    UnderPrime();//언더프라임
    return 0;
}

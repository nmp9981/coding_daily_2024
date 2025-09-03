#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int maxi = 1000;
bool isPrime[maxi];
vector<int> primeList;//소수 목록 
int primeListSize;//소수 개수

//에라토스테네스의 체
void Eratosthenes(){
    //초기화
    int rootMaxi = 32;
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

//3개 소수 합
void SumThreePrime(int n){
    for(int i=0;i<primeListSize;i++){
        for(int j=0;j<primeListSize;j++){
            for(int k=0;k<primeListSize;k++){
                if(primeList[i]+primeList[j]+primeList[k]==n){
                    cout<<primeList[i]<<" "<<primeList[j]<<" "<<primeList[k]<<"\n";
                    return;
                }
            }
        }
    }
    //불가능
    cout<<0<<"\n";
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
  
    Eratosthenes();
  
    int test;
    cin>>test;
    while(test--){
        int n;
        cin>>n;
        SumThreePrime(n);
    }

    return 0;
}

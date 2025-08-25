#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

int n;
int primeSize;
const int maxi = 1000001;
bool isPrime[maxi];//소수 여부
vector<int> primeList;//소수 리스트

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
    primeSize = primeList.size();
}

int main()
{
    Eratosthenes();//소수를 미리 구해놓는다.
    
    int test;
    cin>>test;
    
    while(test--){
        int n;
        cin>>n;
        
        int left = 0;
        int right = primeSize-1;
        int cnt = 0;
        while(left<=right && right<primeSize){
            if(primeList[left]+primeList[right]<=n){
                if(primeList[left]+primeList[right]==n) cnt++;
                
                left++;
            }else right--;
        }
        cout<<cnt<<"\n";
    }

    return 0;
}

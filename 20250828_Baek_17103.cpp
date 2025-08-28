#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int maxi = 1000000;
bool isPrime[maxi];
vector<int> primeList;//소수 목록 
int primeListSize;//소수 개수

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
    primeListSize = primeList.size();//소수 총 개수
}

//파티션 개수 세기
void CountPartition(int x){
    int leftIdx = 0;
    int rightIdx = primeListSize-1;
    int cnt = 0;//개수
  
   //두 포인터
    while(leftIdx<=rightIdx && rightIdx<primeListSize){
        if(primeList[leftIdx]+primeList[rightIdx]<=x){//합이 작으니까 늘려야함
            //골드 바흐 파티션
            if(primeList[leftIdx]+primeList[rightIdx]==x){
                cnt++;
            }
            leftIdx++;
        }else rightIdx--;//합이 크니까 줄여야함
    }
    
    cout<<cnt<<"\n";//출력
}

//입력
void Input(){
    int test;
    cin>>test;
    while(test--){
        int n;
        cin>>n;
        CountPartition(n);//파티션 개수 세기
    }
}

int main()
{
    Eratosthenes();//소수 등록
    Input();//입력
    return 0;
}

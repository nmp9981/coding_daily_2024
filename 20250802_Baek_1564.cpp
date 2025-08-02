#include <iostream>
using namespace std;
typedef long long ll;

ll n;
const ll mod = 1000000000000;//계산 중간에 저장할 숫자
const ll finalMod = 100000;//최종 결과 끝 5자리

//입력
void Input(){
    cin>>n;
}

//팩토리얼5
ll Fact5(){
    ll f5 = 36288;
    
    for(ll i=10;i<=n;i++){
        f5*=i;
        
        //끝에 0들 제거
        while(f5%10==0){
            f5/=10;
        }
        f5%=mod;
    }
    return f5%finalMod;
}

//답 출력
void Answer(ll fac5){
    string facString = to_string(fac5);
    while(facString .length()!=5){
        facString = "0"+facString;
    }
    cout<<facString;
}

int main()
{
    Input();//입력
    ll fact5Result = Fact5();//팩토리얼5
    Answer(fact5Result);//답 출력
    return 0;
}

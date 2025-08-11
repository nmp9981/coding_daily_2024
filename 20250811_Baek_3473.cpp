#include <iostream>
using namespace std;

int test;

//초콜릿 구매
void Fact5Count(int n){
    int cnt = 0;
    for(int i=5;i<=n;i*=5){
        cnt += (n/i);
    }
    cout<<cnt<<"\n";
}

//입력
void Input(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin>>test;
    while(test--){
        int n;cin>>n;
        Fact5Count(n);
    }
}

int main()
{
    Input();//입력
    return 0;
}

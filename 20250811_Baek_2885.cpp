#include <iostream>
using namespace std;

int k, chocoSize;

//초콜릿 구매
void BuyChocolate(){
    chocoSize = 1;
    while(chocoSize < k){
        chocoSize*=2;
    }
}

//초콜릿 나누기
void DivChocolate(){
    //나누지 않음
    if(chocoSize == k){
        cout<<chocoSize<<" "<<0;
        return;
    }
    
    //초콜릿 나누기
    int cnt = 0;
    bool isCount = false;
    while(k>0){
        int rest = k%2;
        if(rest == 1) isCount = true;
        if(isCount) cnt++;
     
        k/=2;
    }
    cout<<chocoSize<<" "<<cnt;
}

//입력
void Input(){
    cin>>k;
}

int main()
{
    Input();//입력
    BuyChocolate();//초콜릿 구매
    DivChocolate();//초콜릿 사기
    return 0;
}

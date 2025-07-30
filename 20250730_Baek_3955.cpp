#include <iostream>
using namespace std;
typedef long long ll;

int n;
ll k,c;
ll x1=1,x2=0,y1=0,buy=1;
const ll inf = 1000000000;

//cx = 1mod(k)
//확장 유클리드 알고리즘
ll ExpandEuclid(ll a,ll b){
    if(b==0) return a;
    
    if(b!=1){
        ll x,y;
        x = x1-((a/b)*x2);
        y=y1-((a/b)*buy);
        x1=x2;y1=buy;x2=x;buy=y;
    }
    return ExpandEuclid(b, a%b);
}

//예외처리
bool IsException(ll k, ll c){
    if(c==1){
        if(k+1>inf){
            cout<<"IMPOSSIBLE\n";
        }else{
            cout<<k+1<<"\n";
        }
        return true;
    }
    if(k==1){//1명 참여 -> 전부 나눠줌
        cout<<1<<"\n";
        return true;
    }
    return false;
}

//입력
void Input(){
    cin>>n;
    for(int i=0;i<n;i++){
        cin>>k>>c;
        if(IsException(k,c)){
            continue;
        }
        
        if(ExpandEuclid(k,c)!=1){
            cout<<"IMPOSSIBLE\n";
        }else{
            x1=1;x2=0;y1=0;buy=1;//변수 초기화
            
            ExpandEuclid(k,c);//유클리드 알고리즘
            
            //양수
            while(buy<0){
                buy+=k;
            }
            
            //결과
            if(buy>inf || buy==0) cout<<"IMPOSSIBLE\n";
            else cout<<buy<<"\n";
        }
    }
}

int main()
{
    Input();
    return 0;
}

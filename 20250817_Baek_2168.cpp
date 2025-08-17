#include <iostream>
#include <cstring>
using namespace std;

int x,y;

//GCD
int GCD(int a,int b){
    if(b==0) return a;
    else return GCD(b,a%b);
}

//입력
void Input(){
    cin>>x>>y;
}

//출력
void Output(){
    //단위 개수
    int gcd = GCD(x,y);
    int unitX = x/gcd;
    int unitY = y/gcd;
    
    cout<<gcd*(unitX+unitY-1);
}

int main()
{
    Input();//입력
    Output();//출력
    return 0;
}

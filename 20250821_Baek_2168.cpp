#include <iostream>
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
    int gcd = GCD(x,y);
    cout<<gcd*(x/gcd+y/gcd-1);
}

int main()
{
    Input();//입력
    Output();//출력
    return 0;
}

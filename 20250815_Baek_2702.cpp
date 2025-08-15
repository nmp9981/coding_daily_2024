#include <iostream>
using namespace std;

//유클리드 알고리즘
int GCD(int a,int b){
    if(b==0) return a;
    else return GCD(b,a%b);
}

int main()
{
    int test;
    cin>>test;
    while(test--){
        int a,b;
        cin>>a>>b;
        
        int gcd = GCD(a,b);
        cout<<(a*b)/gcd<<" "<<gcd<<"\n";
    }
    return 0;
}

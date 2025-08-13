#include <iostream>
using namespace std;

int n,m;

//GCD
int Euclidean(int a, int b)
{
    if(b==0) return a;
    else return Euclidean(b,a%b);
}

//입력
void Input(){
    string s;
    cin>>s;
    
    int findIdx = s.find(":");
    n = stoi(s.substr(0,findIdx));
    m = stoi(s.substr(findIdx+1,-1));
}

//출력
void Output(){
    int gcd = Euclidean(n,m);
    int gcdN = n/gcd;
    int gcdM = m/gcd;
    
    cout<<gcdN<<":"<<gcdM;
}

int main()
{
    Input();
    Output();
    return 0;
}

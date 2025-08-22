#include <iostream>
using namespace std;

string n;

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin>>n;
    if(n=="1"){
        cout<<2;
    }else{
        cout<<1;
    }
    return 0;
}

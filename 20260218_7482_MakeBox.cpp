#include <iostream>
#include <iomanip>
using namespace std;

int n;
int main()
{
    cin>>n;
    for(int i=0;i<n;i++){
        double a;
        cin>>a;
        cout<<fixed<<setprecision(10)<<a/6<<"\n";
    }

    return 0;
}

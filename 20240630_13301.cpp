#include <iostream>
using namespace std;

int n;
const int maxi = 82;
long long PiboNum[maxi];
long long Area;

void Pibonacci(){
    PiboNum[0] = 0;
    PiboNum[1] = 1;
    for(int i=2;i<maxi;i++){
        PiboNum[i] = PiboNum[i-1]+PiboNum[i-2];
    }
}
int main()
{
    cin>>n;
    Pibonacci();
    Area = 2*(PiboNum[n]+PiboNum[n+1]);
    cout<<Area;
    return 0;
}

#include <iostream>
using namespace std;
typedef long long ll;

const int maxi = 101;
const ll mod = 1000000007;
ll StickCount[maxi][maxi][maxi];

void Stick(){
    StickCount[1][1][1] = 1;
    for(int stick=2;stick<maxi;stick++){
        for(int left=1;left<=stick;left++){
            for(int right=1;right<=stick;right++){
                StickCount[stick][left][right] = 
                (StickCount[stick-1][left-1][right]+StickCount[stick-1][left][right-1])%mod+(stick-2)*StickCount[stick-1][left][right]%mod;
                StickCount[stick][left][right]%=mod;
            }
        }
    }
}
int main()
{
    Stick();
    
    int stickNum,l,r;
    cin>>stickNum>>l>>r;
    cout<<StickCount[stickNum][l][r]<<"\n";
    return 0;
}

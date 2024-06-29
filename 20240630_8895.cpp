#include <iostream>
using namespace std;
typedef long long ll;

const int maxi = 21;
ll StickCount[maxi][maxi][maxi];

void Stick(){
    StickCount[1][1][1] = 1;
    for(int stick=2;stick<maxi;stick++){
        for(int left=1;left<=stick;left++){
            for(int right=1;right<=stick;right++){
                StickCount[stick][left][right] = StickCount[stick-1][left-1][right]+StickCount[stick-1][left][right-1]+(stick-2)*StickCount[stick-1][left][right];
            }
        }
    }
}
int main()
{
    Stick();
    
    int t;cin>>t;
    int stickNum,l,r;
    while(t--){
        cin>>stickNum>>l>>r;
        cout<<StickCount[stickNum][l][r]<<"\n";
    }
    return 0;
}

#include <iostream>
using namespace std;

int n;
const int maxi = 27;
int prime[maxi] = {2,3,5,7,11,13,17,19,23,29,
    31,37,41,43,47,53,59,61,67,71,
    73,79,83,89,97,101,103
};

//입력
void Input(){
    //빠른 입력
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin>>n;
}

int main()
{
    Input();//입력
    for(int i=0;i<maxi-1;i++){
        int ans = prime[i]*prime[i+1];
        if(n < ans){
            cout<<ans;
            break;
        }
    }
    return 0;
}

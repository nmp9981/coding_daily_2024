#include <string>
#include <vector>

using namespace std;

const int mod = 10007;
const int maxi = 100001;
int dp[maxi][2];//0 : 영역 침범X, 1 : 영역 침범O

int solution(int n, vector<int> tops) {
    
    //초기값
    dp[0][0]=(tops[0]==0)?2:3;
    dp[0][1]=1;
    
    //타일 설치
    for(int i=1;i<n;i++){
        int top = tops[i];
        
        if(top==0){
            dp[i][0] = 2*dp[i-1][0]+dp[i-1][1];
            dp[i][1] = dp[i-1][0]+dp[i-1][1];
        }else{
            dp[i][0] = 3*dp[i-1][0]+2*dp[i-1][1];
            dp[i][1] = dp[i-1][0]+dp[i-1][1];
        }
        
        dp[i][0] %= mod;
        dp[i][1] %= mod;
    }
    
    return (dp[n-1][0]+dp[n-1][1])%mod;
}

#include <iostream>
using namespace std;

const int maxi =1001;
int pointCount[maxi];//보이는 점의 개수

//GCD
int GCD(int a, int b){
    int r = a%b;
    while(r != 0) {
      r = a % b;
      a = b;
      b = r;
    }
    return a;
}

//1~n까지의 n과 서로소의 개수
int NRealativePrime(int n){
    int cnt = 1;
    for(int i=2;i<n;i++){
        int gcd = GCD(n,i);
        if(gcd==1) cnt++;
    }
    return cnt;
}

//보이는 점의 개수
void CountSeePoint(){
    pointCount[1] = 3;
    for(int i=2;i<maxi;i++){
        pointCount[i] = pointCount[i-1]+2*NRealativePrime(i);
    }
}

//입력
void Input(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    int c;
    cin>>c;
    while(c--){
        int n;
        cin>>n;
        cout<<pointCount[n]<<"\n";
    }

}

int main()
{
    CountSeePoint();//보이는 점의 개수
    Input();//입력
    return 0;
}

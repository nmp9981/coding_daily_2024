#include <iostream>
using namespace std;

int n;

int count5(int n){
    int cnt = 0;
    int divNum = 5;
    while(true){
        if(n/divNum==0){
            break;
        }
        cnt += (n/divNum);
        divNum*=5;
    }
    return cnt;
}

int main()
{
    cin>>n;
    
    int res = -1;
    int left = 1;
    int right = 500000000;

    while(left<=right){
        int nowCount = 0;
        int mid = (left+right)/2;
        
        nowCount = count5(mid);
        if(n<=nowCount){
            if(n==nowCount){
                res = mid;
            }
            right = mid-1;
        }else{
            left = mid+1;
        }
    }
    cout<<res;

    return 0;
}

#include <string>
#include <vector>
using namespace std;

int Count1(int x){
    int cnt= 0;
    while(x>0){
        x = x&(x-1);
        cnt++;
    }
    return cnt;
}
int solution(int n) {
    int inputCount = Count1(n);
    while(true){
        n++;
        if(inputCount==Count1(n)) break;
    }
    return n;
}

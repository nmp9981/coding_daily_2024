#include <string>
#include <vector>
#include <queue>
using namespace std;

const int maxi = 1000001;
int visit[maxi];//각 숫자에 올 최소 연산 횟수

//초기화
void init(){
    for(int i=0;i<maxi;i++) visit[i] = -1;
}
//숫자 변환
int ChangeNum(int x, int y, int n){
    visit[x] = 0;
    queue<int> q;
    q.push(x);
    
    while(!q.empty()){
        int cur = q.front();
        q.pop();
        
        if(cur==y) return visit[cur];//목표 도달
        
        if(cur+n<maxi && visit[cur+n]==-1){//+n
            q.push(cur+n);
            visit[cur+n] = visit[cur]+1;
        }
         if(cur*2<maxi && visit[cur*2]==-1){//*2
            q.push(cur*2);
            visit[cur*2] = visit[cur]+1;
        }
         if(cur*3<maxi && visit[cur*3]==-1){//*3
            q.push(cur*3);
            visit[cur*3] = visit[cur]+1;
        }
    }
    return -1;//y 도달 불가
}
int solution(int x, int y, int n) {
    init();//초기화
    return ChangeNum(x,y,n);//숫자 변환
}

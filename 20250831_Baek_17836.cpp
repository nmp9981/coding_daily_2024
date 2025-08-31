#include <iostream>
#include <cstring>
#include <vector>
#include <queue>
#include <cmath>
using namespace std;

int n,m,t;
const int maxi = 101;
const int inf = 10000000;
int gramTime = inf;
int minTime = inf;
int world[maxi][maxi];//지도
int visitTime[maxi][maxi];//방문 시간
//4방 탐색
int dx[4] = {-1,1,0,0};
int dy[4] = {0,0,-1,1};

//공주님 구출
void SavePrincess(){
    //초기화
    memset(visitTime,-1,sizeof(visitTime));
    queue<pair<int,int>> q;
    q.push({0,0});
    visitTime[0][0] = 0;
    
    while(!q.empty()){
        int curX = q.front().first;
        int curY = q.front().second;
        q.pop();
        
        //그람
        if(world[curX][curY]==2){
            gramTime = visitTime[curX][curY] + abs(n-1-curX)+abs(m-1-curY);
        }
        
        //도착
        if(curX==n-1 && curY==m-1) {
            minTime = visitTime[n-1][m-1];
            return;
        }
        
        //다음 지점
        for(int dir=0;dir<4;dir++){
            int nextX = curX + dx[dir];
            int nextY = curY + dy[dir];
            
            //지도 범위
            if(nextX < 0 || nextX >= n || nextY < 0 || nextY>=m) continue;
            //이미 방문
            if(visitTime[nextX][nextY]!=-1) continue;
            //장애물
            if(world[nextX][nextY]==1) continue;
            
            visitTime[nextX][nextY] = visitTime[curX][curY]+1;
            q.push({nextX, nextY});
        }
    }
}

//입력
void Input(){
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    
    cin>>n>>m>>t;
    for(int i=0;i<n;i++){
        for(int j=0;j<m;j++){
            cin>>world[i][j];
        }
    }
}

//결과 체크
void ResultCheck(){
    minTime = min(gramTime, minTime);
    
    //시간 초과
    if(minTime<=t) cout<<minTime;
    else cout<<"Fail";
}


int main()
{
    Input();//입력
    SavePrincess();//공주님 구출
    ResultCheck();//결과 체크
    return 0;
}

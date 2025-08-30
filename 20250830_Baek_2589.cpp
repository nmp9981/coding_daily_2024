#include <iostream>
#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

int row,col;
const int maxi = 51;
string world[maxi];
int visit[maxi][maxi];
//이동 방향
int dx[4] = {-1,1,0,0};
int dy[4] = {0,0,-1,1};

//BFS
int BFS(int stX, int stY){
    //초기화
    memset(visit,-1,sizeof(visit));
    visit[stX][stY] = 0;
    queue<pair<int,int>> q;
    q.push({stX,stY});
    int dist = 0;
    
    while(!q.empty()){
        int curX = q.front().first;
        int curY = q.front().second;
        q.pop();
        
        for(int dir=0;dir<4;dir++){
            int nextX = curX+dx[dir];
            int nextY = curY+dy[dir];
            
            //범위 조건
            if(nextX<0 || nextX>=row || nextY<0 || nextY>=col) continue;
            //방문조건
            if(visit[nextX][nextY]!=-1) continue;
            //바다
            if(world[nextX][nextY]=='W') continue;
            
            visit[nextX][nextY] = visit[curX][curY]+1;
            dist = max(dist, visit[nextX][nextY]);
            q.push({nextX, nextY});
        }
    }
    return dist;
}

//섬 탐색
void SearchIsland(){
    int maxDist = 0;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            //바다일 경우 pass
            if(world[i][j]=='W') continue;
            maxDist = max(maxDist, BFS(i,j));
        }
    }
    cout<<maxDist;
}

//입력
void Input(){
    cin>>row>>col;
    for(int i=0;i<row;i++){
        cin>>world[i];
    }
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);

    Input();//입력
    SearchIsland();//섬 탐색
    return 0;
}

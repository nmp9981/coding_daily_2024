#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int maxi = 101;
bool visit[maxi][maxi];//방문 여부
vector<int> answer;//정답 배열
int dx[4] = {-1,1,0,0};
int dy[4] = {0,0,-1,1};
int row,col;

//BFS
int BFSSearch(int startX,int startY, vector<string> maps){
    queue<pair<int,int>> q;
    q.push({startX,startY});
    int cnt = maps[startX][startY]-'0';
    visit[startX][startY] = true;
    
    while(!q.empty()){
        int x = q.front().first;
        int y = q.front().second;
        q.pop();
        
        for(int k=0;k<4;k++){
            int nx = x+dx[k];
            int ny = y+dy[k];
            if(nx<0 || nx>=row || ny<0 || ny>=col) continue;
            if(visit[nx][ny]) continue;
            if(maps[nx][ny]=='X') continue;
            
            visit[nx][ny] = true;
            q.push({nx,ny});
            cnt+= (maps[nx][ny]-'0');
        }
    }
    return cnt;
}
//미방문 지역 탐색
void Travel(vector<string> maps){
    row = maps.size();
    col = maps[0].size();
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            if(!visit[i][j] && maps[i][j]!='X') {
                int day = BFSSearch(i,j,maps);
                answer.push_back(day);
            }
        }
    }
    if(answer.size()==0) answer.push_back(-1);
    else sort(answer.begin(), answer.end());
}

vector<int> solution(vector<string> maps) {
    Travel(maps);
    return answer;
}

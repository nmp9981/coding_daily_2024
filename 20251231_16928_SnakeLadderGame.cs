#include <iostream>
#include <cstring>
#include <map>
#include <queue>

using namespace std;

int n,m;
const int maxi = 101;
const int minPos = 1;
const int maxPos = 100;
bool isVisit[maxi];//방문 여부
map<int,int> moveDic;//이동
queue<pair<int,int>> posQueue;//플레이어 위치, 플레이 횟수를 담는 큐

//초기화
void Init(){
    memset(isVisit, false, sizeof(isVisit));
}

//입력
void Input(){
    cin>>n>>m;
    for(int i=0;i<n+m;i++){
        int x,y;
        cin>>x>>y;
        moveDic.insert({x,y});
    }
}

//게임 플레이
void PlayGame(){
    //맨 처음엔 1번 부터 시작
    int pos = 1;
    isVisit[pos]=true;
    posQueue.push({pos,0});
    
    while(!posQueue.empty()){
        //현 위치, 횟수
        int curPos = posQueue.front().first;
        int curCount = posQueue.front().second;
        posQueue.pop();
        
        //100번 도착
        if(curPos==100){
            cout<<curCount;
            return;
        }
        
        //다음 주사위 굴리기
        for(int del=1;del<7;del++){
            //다음 위치
            int nextPos = curPos+del;
            if(moveDic.find(nextPos)!=moveDic.end()){//사다리 or 뱀
                nextPos = moveDic[nextPos];
            }
            
            //조건 체크
            if(isVisit[nextPos]) continue;//이미 방문
            if(nextPos<minPos || nextPos > maxPos) continue;//보드판 범위 벗어남
            
            //다음 위치로
            isVisit[nextPos] = true;
            posQueue.push({nextPos, curCount+1});
        }
    }
}

int main()
{
    Init();//초기화
    Input();//입력
    PlayGame();//게임 플레이
    return 0;
}

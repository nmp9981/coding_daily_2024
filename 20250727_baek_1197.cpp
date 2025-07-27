#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

int v,e;
const int maxi = 10001;
int parent[maxi];
vector<pair<int,pair<int,int>>> graph;

//입력
void input(){
   cin>>v>>e;
   for(int i=0;i<e;i++){
       int a,b,c;
       cin>>a>>b>>c;
       graph.push_back({c,{a,b}});//가중치 기준으로 정렬
   } 
   sort(graph.begin(),graph.end());
}

//초기화
void init(){
    for(int i=0;i<maxi;i++){
       parent[i] = i;
    }
}

//각 노드의 루트찾기
int Find(int x){
    if(parent[x] == x){
        return x;
    }
    return parent[x] = Find(parent[x]);
}

//그래프 합치기
void Merge(int x,int y){
    if(x<y) parent[y] = x;
    else parent[x] = y;
}

//스패닝 트리
void Spanning(){
    int node = 0;
    int total = 0;
    for(int i=0;i<graph.size();i++){
        int st = graph[i].second.first;
        int ed = graph[i].second.second;
        int dist = graph[i].first;
        
        int fx = Find(st);
        int fy = Find(ed);
        //류투노두가 서로 다를 경우 합치기
        if(fx!=fy){
            Merge(fx,fy);
            node+=1;
            total+=dist;
        }
        
        //완료
        if(node==v-1){
            cout<<total;
            break;
        }
     }
}

int main(){
    //빠른 입력
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    init();
    input();
    Spanning();
    return 0;
}

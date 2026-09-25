#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// 두 번째 원소 기준 오름차순, 같으면 첫 번째 원소 기준 오름차순
bool cmp(const vector<int>& a, const vector<int>& b) {
    if (a[1] == b[1]) {
        return a[0] < b[0];
    }
    return a[1] < b[1];
}

int solution(vector<vector<int>> targets) {
    int answer = 1;//기본 1개
    //끝점 기준으로 오름차순 정렬
    sort(targets.begin(), targets.end(), cmp);
    
    //미사일 탐색
    int missileCount = targets.size();
    int endPoint = targets[0][1];//종료 지점
    for(int i=1;i<missileCount;i++){
        if(endPoint <= targets[i][0]){//기존 범위 초과
            endPoint = targets[i][1];//종료 지점 갱신
            answer++;//미사일 개수 초과
        }
    }
    return answer;
}

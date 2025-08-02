#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;
typedef long long ll;

//좌표 구조체
struct Point{
    int x,y;
};

int n;
const int maxi = 100001;
vector<Point> pos;//점들 배열
Point pivot;//기준점

// CCW 함수: 세 점의 방향을 판별
// 1: 반시계 방향 (좌회전)
// -1: 시계 방향 (우회전)
// 0: 일직선
int ccw(Point p1, Point p2, Point p3) {
    long long val = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    if (val == 0) return 0;
    return (val > 0) ? 1 : -1;
}

// 점들의 정렬 기준: 반시계 방향 각도
bool comparePoints(Point p1, Point p2) {
    int orientation = ccw(pivot, p1, p2);
    if (orientation == 0) {//일직선
        // 각도가 같으면 피봇과의 거리가 더 가까운 점을 앞으로
        int p1Dist = (p1.x - pivot.x) * (p1.x - pivot.x) + (p1.y - pivot.y) * (p1.y - pivot.y);
        int p2Dist = (p2.x - pivot.x) * (p2.x - pivot.x) + (p2.y - pivot.y) * (p2.y - pivot.y);
        return p1Dist < p2Dist;
    }
    return orientation > 0;
}

// 그라함 스캔 알고리즘
vector<Point> GrahamScan(vector<Point> points) {
    // 1단계: 가장 낮은 y좌표를 가진 점을 찾기 (pivot 설정)
    //y가 같으면 x좌표가 더 작은것으로 설정
    int minIndex = 0;
    for (int idx=0;idx<n;idx++) {
        if (points[idx].y < points[minIndex].y ||
            (points[idx].y == points[minIndex].y && points[idx].x < points[minIndex].x)) {
            minIndex = idx;
        }
    }
    swap(points[0], points[minIndex]);
    pivot = points[0];

    // 2단계: 기준점을 중심으로 점들을 정렬
    sort(points.begin() + 1, points.end(), comparePoints);

    // 3단계: 스택을 사용하여 볼록 껍질 구성
    stack<Point> hullStack;
    hullStack.push(points[0]);
    hullStack.push(points[1]);

    for (int idx=2;idx<n;idx++) {
        while (hullStack.size() >= 2) {
            //맨위 2개
            Point p2 = hullStack.top();
            hullStack.pop();
            Point p1 = hullStack.top();
            
            //반시계 방향
            if (ccw(p1, p2, points[idx]) > 0) {
                hullStack.push(p2);
                break;
            }
        }
        hullStack.push(points[idx]);
    }

    // 스택의 결과를 벡터로 변환
    vector<Point> convexHull;
    while (!hullStack.empty()) {
        convexHull.push_back(hullStack.top());
        hullStack.pop();
    }
    reverse(convexHull.begin(), convexHull.end());

    return convexHull;
}

//입력
void Input(){
    ios::sync_with_stdio(0);
	cin.tie(0);
	
	cin>>n;
	for(int i=0;i<n;i++){
	    int x,y;
	    cin>>x>>y;
	    pos.push_back({x,y});
	}
}

int main()
{
    Input();//입력
    vector<Point> convexHull = GrahamScan(pos, n);//컨벡스 헐
    cout<<convexHull.length();
    return 0;
}

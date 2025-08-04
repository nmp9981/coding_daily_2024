#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;
typedef long long ll;

//좌표 구조체
struct Point {
    ll x, y;
};

int n;
vector<Point> pos;//점들 배열
Point pivot;//기준점

// CCW 함수: 세 점의 방향을 판별
// -1: 반시계 방향 (좌회전)
// 1: 시계 방향 (우회전)
// 0: 일직선
ll ccw(Point p1, Point p2, Point p3) {
    ll val = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
    if (val == 0) return 0;
    return (val > 0) ? -1 : 1;
}

// 점들의 정렬 기준: 반시계 방향 각도
bool comparePoints(Point p1, Point p2) {
    ll orientation = ccw(pivot, p1, p2);
    if (orientation == 0) {//일직선
        // 각도가 같으면 거리가 가까운 점이 먼저 오도록 정렬
        ll p1Dist = (p1.x - pivot.x) * (p1.x - pivot.x) + (p1.y - pivot.y) * (p1.y - pivot.y);
        ll p2Dist = (p2.x - pivot.x) * (p2.x - pivot.x) + (p2.y - pivot.y) * (p2.y - pivot.y);
        return p1Dist < p2Dist;
    }
    return orientation < 0;//반시계 방향이 우선
}

// 두 점 사이의 거리의 제곱을 계산
ll distSq(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
}

// 그라함 스캔 알고리즘
vector<Point> GrahamScan(vector<Point> points) {
    int n = points.size();
    if (n < 3) return points;//컨벡스헐 구성 불가

    // 1단계: 가장 낮은 y좌표를 가진 점을 찾기 (pivot 설정)
    //y가 같으면 x좌표가 더 작은것으로 설정
    int minIndex = 0;
    for (int idx = 1; idx < n; idx++) {
        if (points[idx].y < points[minIndex].y ||
            (points[idx].y == points[minIndex].y && points[idx].x < points[minIndex].x)) {
            minIndex = idx;
        }
    }
    swap(points[0], points[minIndex]);
    pivot = points[0];

    // 2단계: 기준점을 중심으로 점들을 정렬
    sort(points.begin() + 1, points.end(), comparePoints);

    // 3단계: 정렬 후 일직선상에 있는 중복 점들 제거 (가장 먼 점만 남김)
    int m = 1;
    for (int idx = 1; idx < n; idx++) {
        //일직선이 아닐때까지 계속
        while (idx < n - 1 && ccw(pivot, points[idx], points[idx + 1]) == 0) {
            idx++;
        }
        points[m] = points[idx];
        m++;
    }
    points.resize(m);
    n = m;
    if (n < 3) return {};


    // 4단계: 스택을 사용하여 볼록 껍질 구성
    stack<Point> hullStack;
    hullStack.push(points[0]);
    hullStack.push(points[1]);

    for (int idx = 2; idx < n; idx++) {
        while (hullStack.size() >= 2) {
            Point p2 = hullStack.top();
            hullStack.pop();
            Point p1 = hullStack.top();

            // p1, p2, points[idx]의 방향을 확인합니다.
            if (ccw(p1, p2, points[idx]) < 0) { // 반시계 방향일 경우
                hullStack.push(p2); // p2를 다시 스택에 넣고
                break; // while 루프를 종료합니다.
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

//가장 거리가 먼 점
void MostFarPoints(vector<Point> points) {
    int n = points.size();
    int p1, p2;
    ll maxdist;

    p1 = 0, p2 = 1;
    Point p1_res = points[0], p2_res = points[1];
    maxdist = distSq(points[p1], points[p2]);
    
    for (int i = 0; i < n * 2; i++) {
        int np1 = (p1 + 1) % n;
        int np2 = (p2 + 1) % n;

        Point a = { points[np1].x - points[p1].x, points[np1].y - points[p1].y };
        Point b = { points[np2].x - points[p2].x, points[np2].y - points[p2].y };
        ll tmp = a.x * b.y - a.y * b.x;
       
        //더 작은 각도만큼 캘리퍼스 회전
        if (tmp > 0) p1 = np1;
        if (tmp < 0) p2 = np2;
        if (!tmp) { p1 = np1, p2 = np2; }

        //현재 두 점 사이의 거리가 더 길면 갱신하기
        ll nowdist = distSq(points[p1] , points[p2]);
        if (maxdist < nowdist) {
            maxdist = nowdist;
            p1_res = points[p1];
            p2_res = points[p2];
        }
    }
    // 결과 출력
    cout << p1_res.x << " " << p1_res.y << " " << p2_res.x << " " << p2_res.y << "\n";
}

//입력
void Input() {
    
    cin >> n;
    for (int i = 0; i < n; i++) {
        ll x, y;
        cin >> x >> y;
        pos.push_back({ x,y });
    }
}

int main()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    int test;
    cin >> test;
    while (test--) {
        pos.clear();
        Input();//입력
        vector<Point> convexHull = GrahamScan(pos);//컨벡스 헐
        MostFarPoints(convexHull);//가장 먼점
    }

    return 0;
}

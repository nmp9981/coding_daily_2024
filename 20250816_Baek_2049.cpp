#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <cmath>
using namespace std;
typedef long long ll;

//좌표 구조체
struct Point {
    ll x, y;
};

int n;//입력 점 개수
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

// 두 점 사이의 거리 제곱
long long dist_sq(Point a, Point b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// 두 벡터의 외적을 계산하는 함수 (시계 반대 방향 여부 판단)
long long cross_product(Point a, Point b, Point c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
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

// 주어진 점 집합에서 볼록 껍질을 계산하는 함수
vector<Point> JarvisMarch(vector<Point> points) {
    int n_size = points.size();
    if (n < 3) return {}; // 볼록 껍질을 만들 수 없음

    // 1단계: y좌표가 가장 작은 점을 찾아서 시작점으로 설정
    int startIdx = 0;//y좌표가 가장 작은 점의 인덱스
    for (int i = 1; i < n_size; i++) {
        //y좌표가 같은 경우 x좌표가 더 작은 점으로
        if (points[i].y < points[startIdx].y || (points[i].y == points[startIdx].y && points[i].x < points[startIdx].x)) {
            startIdx = i;
        }
    }

    //볼록 껍질 제작
    vector<Point> hull;//볼록 껍질 점들
    int curPoint = startIdx;

    do {
        // 2단계: 현재 점을 볼록 껍질에 추가
        hull.push_back(points[curPoint]);

        // 3단계: 다음 점을 찾기
        int nextPoint = (curPoint + 1) % n;
        // 모든 점들을 순회하며 가장 반시계 방향에 있는 점을 찾음
        for (int i = 0; i < n_size; ++i) {
            int direction = ccw(points[curPoint], points[nextPoint], points[i]);

            // `points[i]`가 `nextPoint`보다 반시계 방향일 경우 갱신
            if (direction == -1) {
                nextPoint = i;
            }
            // 세 점이 일직선일 경우, 더 먼 점을 다음 점으로 선택
            else if (direction == 0) {
                if (dist_sq(points[curPoint], points[i]) > dist_sq(points[curPoint], points[nextPoint])) {
                    nextPoint = i;
                }
            }
        }

        curPoint = nextPoint;

    } while (curPoint != startIdx); // 다음 점이 시작점과 같아질 때까지 반복

    return hull;
}


// 그라함 스캔 알고리즘
vector<Point> GrahamScan(vector<Point> points) {
    int n_size = points.size();
    if (n_size < 3) return points;//컨벡스헐 구성 불가

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
    int finalNum = 1;//최종 점들 번호(0은 시작점 이므로 1번 부터 시작)
    for (int idx = 1; idx < n_size; idx++) {
        //일직선이 아닐때까지 계속
        while (idx < n_size - 1 && ccw(pivot, points[idx], points[idx + 1]) == 0) {
            idx++;
        }
        points[finalNum] = points[idx];//일직선상의 맨 끝점
        finalNum++;//다음 번호
    }
    points.resize(finalNum);//정렬
    n_size = finalNum;//최종 사이즈(최종 점 개수)
    //최종 점의 개수가 3개 미만이면 컨벡스 헐을 만들 수 없으므로 빈 벡터 반환
    if (n_size < 3) return {};


    // 4단계: 스택을 사용하여 볼록 껍질 구성
    stack<Point> hullStack;
    hullStack.push(points[0]);
    hullStack.push(points[1]);

    for (int idx = 2; idx < n_size; idx++) {
        while (hullStack.size() >= 2) {//맨위 2개를 뺀다.
            Point p2 = hullStack.top();//맨위
            hullStack.pop();
            Point p1 = hullStack.top();//2번째 위

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

//회전하는 캘리퍼스
ll RotatingCalipers(vector<Point> convex_hull) {
    int nSize = convex_hull.size();

    //거리 정의 불가
    if (nSize < 2) return 0;
    //두 점사이의 거리
    if (nSize == 2) return dist_sq(convex_hull[0], convex_hull[1]);

    //회전하는 캘리퍼스 적용
    ll max_dist_sq = 0;//가장 먼 두점 사이의 거리
    int k = 1;//대척점 인덱스

    for (int i = 0; i < nSize; ++i) {//점들을 순회
        // i와 k가 대척점 쌍이 되도록 k를 탐색
        while (cross_product(convex_hull[i], convex_hull[(i + 1) % nSize], convex_hull[k]) 
            < cross_product(convex_hull[i], convex_hull[(i + 1) % nSize], convex_hull[(k + 1) % nSize])) {
            k = (k + 1) % nSize;
        }

        // i와 k 사이의 거리 갱신
        max_dist_sq = max(max_dist_sq, dist_sq(convex_hull[i], convex_hull[k]));
        max_dist_sq = max(max_dist_sq, dist_sq(convex_hull[(i + 1) % nSize], convex_hull[k]));
    }
    return max_dist_sq;
}

//일직선 검사
bool IsOneLine(vector<Point> pos){
    int n_size = pos.size();
    Point minPos = pos[0];
    Point maxPos = pos[0];
  
    for (int idx = 1; idx < n_size-1; idx++) {
        //일직선이 아닐때까지 계속
        if(ccw(pos[0], pos[idx], pos[idx + 1]) != 0) {
            return false;
        }
        //최소, 최대 좌표
        minPos.x = min(minPos.x, pos[idx].x);
        minPos.y = min(minPos.y, pos[idx].y);
        maxPos.x = max(maxPos.x, pos[idx].x);
        maxPos.y = max(maxPos.y, pos[idx].y);
    }
    //마지막 좌표
    minPos.x = min(minPos.x, pos[n_size-1].x);
    minPos.y = min(minPos.y, pos[n_size-1].y);
    maxPos.x = max(maxPos.x, pos[n_size-1].x);
    maxPos.y = max(maxPos.y, pos[n_size-1].y);
    
    ll dist = dist_sq(minPos ,maxPos);
    cout <<dist;
    return true;
}

//입력
void Input() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;
    for (int i = 0; i < n; i++) {
        ll x, y;
        cin >> x >> y;
        pos.push_back({ x,y });
    }
}

int main()
{
    Input();//입력
    //일직선 검사
    if(!IsOneLine(pos)){
        vector<Point> convexHull = GrahamScan(pos);//컨벡스 헐
        ll farthest_dist = RotatingCalipers(convexHull);//회전하는 캘리퍼스
        cout <<farthest_dist;
    }
    return 0;
}

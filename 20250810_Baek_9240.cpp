#include <iostream>
#include <algorithm>
#include <string.h>
#include <vector>
#include <cmath>

using namespace std;
typedef long long ll;

struct vec {
    int x, y;
    
    vec(){};
    vec(int _x, int _y) : x(_x), y(_y) {};
    
    //벡터용 - 연산자
    vec operator -(const vec &rvec) const { return {x-rvec.x, y-rvec.y}; }
    //벡터용 < 연산자
    bool operator <(const vec &rvec) const {
        if(y != rvec.y) return y < rvec.y;
        return x < rvec.x;
    }
    //벡터 외적
    ll cross(const vec& rvec) const { return 1LL*x*rvec.y - 1LL*rvec.x*y; }
    //두 벡터 사이의 길이
    ll norm() const { return 1LL*x*x + 1LL*y*y; }
};
ll ccw(vec a, vec b) { return a.cross(b); }
ll ccw(vec p, vec a, vec b) { return ccw(a-p, b-p); }

//테스트 케이스의 수 T
int T;
//도시의 개수 n
int n;
//도시의 좌표
vector<vec> p;
//제일 왼쪽 아래 정점
vec s;

//s를 기준으로 반시계방향으로 정렬
bool cmp(vec a, vec b) {
    //같은 직선상에 위치하지 않는다면
    ll tmp = ccw(a-s, b-s);
    if(tmp)
        return tmp > 0;
    return a < b;
}

//convex hull 알고리즘
vector<int> convex_hull(vector<vec>& p) {
    //제일 왼쪽 아래 점 찾기
    swap(p[0], *min_element(p.begin(), p.end()));
    s = p[0];
    //s를 기준으로 반시계방향으로 정렬
    sort(p.begin()+1, p.end(), cmp);
    
    vector<int> ret;
    ret.push_back(0);
    ret.push_back(1);
    
    for(int here=2; here<p.size(); here++) {
        while(ret.size() >= 2) {
            int last1 = *(ret.end() - 1);
            int last2 = *(ret.end() - 2);;
            if(ccw(p[last2], p[last1], p[here]) > 0)
                break;
            ret.pop_back();
        }
        ret.push_back(here);
    }
    
    return ret;
}

//가장 멀리 떨어져 있는 두 점
vec res1, res2;
void rotating_calipers(vector<vec>& p) {
    vector<int> tmp = convex_hull(p);
    vector<vec> cx;
    //convex_hull로 얻은 정점을 cx에 넣기
    for(int e : tmp)
        cx.push_back(p[e]);
    
    int n = cx.size();
    
    int p1, p2;
    ll maxdist;
    
    p1 = 0, p2 = 1;
    maxdist = (cx[p1] - cx[p2]).norm();
    res1 = cx[p1], res2 = cx[p2];
    
    for(int i=0; i<n*2; i++) {
        int np1 = (p1 + 1) % n;
        int np2 = (p2 + 1) % n;
        ll tmp = ccw(cx[np1] - cx[p1], cx[p2] - cx[np2]);
        
        //더 작은 각도만큼 캘리퍼스 회전
        if(tmp>0) p1 = np1;
        if(tmp<0) p2 = np2;
        if(!tmp) { p1 = np1, p2 = np2; }
        
        //현재 두 점 사이의 거리가 더 길면 갱신하기
        ll nowdist = (cx[p1] - cx[p2]).norm();
        if(maxdist < nowdist) {
            maxdist = nowdist;
            res1 = cx[p1];
            res2 = cx[p2];
        }
    }
}

void input() {
    //초기화
    p.clear();
    
    cin >> n;
    
    for(int i=0; i<n; i++) {
        int x, y;
        cin >> x >> y;
        p.push_back(vec(x, y));
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    
    input();
    rotating_calipers(p);
    ll dist = (res2.x-res1.x)* (res2.x-res1.x)+(res2.y-res1.y)* (res2.y-res1.y);
    cout << fixed;
    cout.precision(8);
    cout << sqrt(dist);
    
    return 0;
}

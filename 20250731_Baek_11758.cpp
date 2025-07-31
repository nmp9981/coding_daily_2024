#include <iostream>
using namespace std;

// 2차원 좌표를 나타내는 구조체
struct Point {
    int x, y;
};

//세 점
Point a,b,c;

//CCW : 점C가 벡터AB에 대해 어느 방향에 위치하는지
int CCW(Point A, Point B, Point C){
    int crossValue = (B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x);
    
    //결과
    if(crossValue > 0) return 1;//반시계
    else if(crossValue == 0) return 0;//일직선
    else return -1;//시계
}

//입력
void Input(){
    cin>>a.x>>a.y;
    cin>>b.x>>b.y;
    cin>>c.x>>c.y;
}

int main()
{
    Input();//입력
    cout<<CCW(a,b,c);//CCW
    return 0;
}

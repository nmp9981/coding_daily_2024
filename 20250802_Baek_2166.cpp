#include <iostream>
using namespace std;

//점 구조체
struct Point{
    double x,y;
};

int n;
const int maxi = 10001;
Point points[maxi];

//입력
void Input(){
    cin>>n;
    for(int i=0;i<n;i++){
        cin>>points[i].x>>points[i].y;
    }
}

//면적
void Area(){
    double area = 0.0;
    for(int i=1;i<n-1;i++){
        area += (points[i].x-points[0].x)*(points[i+1].y-points[0].y)
        -(points[i].y-points[0].y)*(points[i+1].x-points[0].x);
    }
    
    area *= 0.5;
    area = area<0?-area:area;
     //소수점 첫째 자리까지 출력
    cout<<fixed;
    cout.precision(1);
    cout<<area;
}

int main()
{
    Input();//입력
    Area();//면적
    return 0;
}

#include <iostream>
#include <cmath>
using namespace std;

int n;
const double Gravity = 9.81;
const double MaxDist = 1501;
const double Upsilon = 1e-6;
const double PI = 3.141592;

//초기 속도
double InitVelocity(double height){
    return sqrt(2*height*Gravity);
}
//언덕 함수
double Hill(double x,double H,double L){
    if(x<0.0) return H;
    if(x<0.5*L){
        double sub = (2*x*x)/(L*L);
        return H*(1-sub);
    }
    if(x<L){
        double sub = x/L-1;
        return 2*H*sub*sub;
    }
    return 0.0;
}
//언덕 함수 미분
double Hill_prime(double x,double H,double L){
    if(x<0.0) return 0.0;
    if(x<0.5*L){
        return ((-4)*x*H)/(L*L);
    }
    if(x<L){
        return (4*H*(x/L-1))/L;
    }
    return 0.0;
}
//스키 선수 이동 함수
double F(double x,double v0,double H,double p){
    return ((-0.5)*Gravity*x*x)/(v0*v0)+p+H;
}
//스키 선수 이동 함수 미분
double F_prime(double x,double v0){
    return ((-1)*Gravity*x)/(v0*v0);
}
//착륙 X위치
double SetDown_XPos(double v0,double p,double H,double L){
    double low = 0.0;
    double high = MaxDist;
    double mid;
    while(high-low > Upsilon){
        mid = (high+low)*0.5;
        if(Hill(mid,H,L) < F(mid,v0,H,p)){//착륙 안함
            low = mid;//더 갈 수 있음
        }else{//착륙 이후
            high = mid;//기록 감소
        }
    }
    return mid;
}

//스키 점프
void SkiJump(double j,double p,double H,double L){
    //초기 속도
    double v0 = InitVelocity(j);
    
    //착륙 위치
    double arriveX = SetDown_XPos(v0,p,H,L);
    double arriveY = F(arriveX,v0,H,p);
    //착륙 속도 (출발 지점으로 부터의 높이)
    double arriveVelocity = sqrt(2*Gravity*(j+p+H-arriveY));
    //착륙 각도 (호도법->60분법 변환 필요)
    //삼각함수 덧셈공식 활용
    double tanAngle = (Hill_prime(arriveX,H,L) - F_prime(arriveX,v0)) / (1 + F_prime(arriveX,v0) * Hill_prime(arriveX,H,L));
    double angle = atan(abs(tanAngle));
    angle = (angle*180)/PI;
    
    //출력
    cout<<fixed;
    cout.precision(8);
    cout<<arriveX<<" "<<arriveVelocity<<" "<<angle<<"\n";
}

//입력
void Input(){
    ios::sync_with_stdio(false); 
    cin.tie(NULL); 

    cin>>n;
    while(n--){
        double j,p,H,L;
        cin>>j>>p>>H>>L;
        SkiJump(j,p,H,L);
    }
}
int main()
{
    Input();//입력
    return 0;
}

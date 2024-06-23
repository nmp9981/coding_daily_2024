#include <string>
#include <vector>
#include <cmath>
using namespace std;

int a,b;//x^2+ax+b=0 의 계수
//방정식 세팅
void EquationSet(int brown, int yellow){
    int mul = yellow;
    int sum = brown/2-2;
    a = -sum;
    b = mul;
}
//방정식 풀이
vector<int> EquationSolve(){
    vector<int> answer;
    int answer1 = (-a+sqrt(a*a-4*b))/2;
    int answer2 = (-a-sqrt(a*a-4*b))/2;
    answer.push_back(answer1+2);
    answer.push_back(answer2+2);
    return answer;
}

vector<int> solution(int brown, int yellow) {
    EquationSet(brown, yellow);
    return EquationSolve();
}

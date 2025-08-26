#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

int G;
vector<int> ableCurWeight;//가능한 현재 몸무게

//다이어트
void Diet(){
    int rootG = sqrt(G);
    for(int i=1;i<=rootG;i++){
        //자연수 범위 인수분해 가능
        if(G%i==0){
            int sumWeight = G/i;
            int diffWeight = i;
            
            //기억 몸무게가 0인 경우는 제외
            if(sumWeight==diffWeight) continue;
            
            //현재 몸무게를 구함(이때 현재 몸무게는 자연수여야함)
            if((sumWeight+diffWeight)%2==0){
                int curWeight = (sumWeight+diffWeight)/2;
                ableCurWeight.push_back(curWeight);
            }
        }
    }
}

//입력
void Input(){
    cin>>G;
}
//출력
void Output(){
    if(ableCurWeight.size()==0) cout<<-1;
    else{
        sort(ableCurWeight.begin(),ableCurWeight.end()); //오름차순 출력
        for(int i=0;i<ableCurWeight.size();i++){
            cout<<ableCurWeight[i]<<"\n";
        }
    }
}

int main()
{
    Input();//입력
    Diet();//다이어트
    Output();//출력
    return 0;
}

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
using namespace std;

int G;
vector<int> ableWeight;//가능한 몸무게

//다이어트
void Diet(){
    int rootG = sqrt(G);
    for(int i=1;i<=rootG;i++){
        if(G%i==0){
            //두 인수
            int maxI = G/i;
            int minI = i;
            
            //몸무게 구하기
            if((maxI+minI)%2==0 && (maxI-minI)%2==0){
                if(maxI-minI==0) continue;//두 몸무게 값은 자연수
                int weight = (maxI+minI)/2;
                ableWeight.push_back(weight);
            }
        }
    }
    sort(ableWeight.begin(), ableWeight.end());
    
    //가능한 몸무게가 없음
    if(ableWeight.size()==0){
        cout<<-1;
        return;
    }
    
    //가능한 몸무게 출력
    for(int i=0;i<ableWeight.size();i++){
        cout<<ableWeight[i]<<"\n";
    }
}

//입력
void Input(){
    cin>>G;
}

int main()
{
    Input();//입력
    Diet();//다이어트
    return 0;
}

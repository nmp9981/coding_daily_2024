#include <string>
#include <vector>
using namespace std;

const int maxSize = 1000001;
int getMoney_First[maxSize];//0번 집을 터는 경우
int getMoney_NotFirst[maxSize];//0번 집을 털지 않는 경우

//Max
inline int Max(int a,int b){
    return a>b?a:b;
}

//0번집 정산
void GetFirstHouse(vector<int> money){
    //0번집을 터는 경우
    getMoney_First[0] = money[0];
    getMoney_First[1] = getMoney_First[0];
    //1번집을 터는 경우
    getMoney_NotFirst[0] = 0;
    getMoney_NotFirst[1] = money[1];
}

//돈 훔치기
void StealHouse(vector<int> money){
    //0번 집을 털면 2번부터 정산, 마지막은 털 수 없음
    for(int i=2;i<money.size()-1;i++){
        getMoney_First[i] = Max(getMoney_First[i-2]+money[i], getMoney_First[i-1]);
    }
    //0번집을 털지 않았다면 1번부터 정산, 마지막을 털 수 있음
    for(int i=2;i<money.size();i++){
        getMoney_NotFirst[i] = Max(getMoney_NotFirst[i-2]+money[i], getMoney_NotFirst[i-1]);
    }
}

int solution(vector<int> money) {
    int answer = 0;
    //0번집
    GetFirstHouse(money);
    
    //나머지
    StealHouse(money);
    
    //최종
    answer = Max(getMoney_First[money.size()-2], getMoney_NotFirst[money.size()-1]);
    return answer;
}

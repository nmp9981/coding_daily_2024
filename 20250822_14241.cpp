#include <iostream>
#include <queue>
using namespace std;

int n;
priority_queue<int,vector<int>,greater<int>>pq; //최소힙

//입력
void Input(){
    cin>>n;
    for(int i=0;i<n;i++){
        int x;
        cin>>x;
        pq.push(x);
    }
}

//점수 계산
void CalScore(){
    int score = 0;
    while(pq.size()>1){
        int num1 = pq.top();
        pq.pop();
        int num2 = pq.top();
        pq.pop();
        
        score+=(num1*num2);
        int newNum = num1+num2;
        pq.push(newNum);
    }
    cout<<score;
}
int main()
{
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    Input();
    CalScore();
    return 0;
}

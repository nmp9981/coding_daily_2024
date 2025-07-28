#include <iostream>
#include <stack>
using namespace std;

int n;
const int maxi = 1001;
int people[maxi];//사람들 번호
stack<int> waitStack;//대기열

//입력
void Input(){
    //빠른 입력
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    
    cin>>n;
    for(int i=0;i<n;i++){
        cin>>people[i];
    }
}

//라인 통과
void LinePath(){
    int curExitNum = 1;//간식을 받을 번호
    
    for(int i=0;i<n;i++){//맨 앞부터 탐색
        int curNum = people[i];
        
        waitStack.push(curNum);
        //대기열 맨 앞줄과 간식을 받을 사람의 번호가 일치하는 동안
        while(waitStack.top()==curExitNum){
            waitStack.pop();
            curExitNum+=1;//다음 간식 배분 번호
            
            //스택이 비었음 -> while문을 나감
            if(waitStack.empty()){
                break;
            }
        }
    }
    
    //최종 검사
    if(waitStack.empty()){//모두가 받음
        cout<<"Nice";
    }else{//받지 못한자 존재, ex) 2 3 1
        cout<<"Sad";
    }
}

int main()
{
    Input();//입력
    LinePath();//라인 통과
    return 0;
}

#include <string>
#include <vector>
using namespace std;

string alpha[5] = {"A","E","I","O","U"};
vector<string> dic;

//단어 세팅
void WordSetting(string word, int depth){
    if(depth==5) return;
    
    for(int i=0;i<5;i++){
        string nextWord = word+alpha[i];
        dic.push_back(nextWord);//단어 등록
        WordSetting(nextWord,depth+1);
    }
}
//단어 찾기
int FindWord(string word){
    for(int i=0;i<dic.size();i++){
        if(dic[i]==word) return i+1;
    }
    return 0;
}
int solution(string word) {
    WordSetting("",0);//단어 세팅
    return FindWord(word);
}

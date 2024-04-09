#include <string>
#include <vector>
using namespace std;

vector<int> Calculate(int n,int s){
    int div = s/n;
    vector<int> res;
    if(div==0) res.push_back(-1);//개수 > 합
    else{
        for(int i=0;i<n;i++) res.push_back(div);
        
        int mod = s%n;
        int idx = res.size()-1;
        while(mod>0){
            res[idx] = div+1;
            idx--;
            mod--;
        }
    }
    return res;
}
vector<int> solution(int n, int s) {
    return Calculate(n,s);
}

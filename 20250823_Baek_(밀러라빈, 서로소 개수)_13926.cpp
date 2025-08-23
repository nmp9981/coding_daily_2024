#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>
using namespace std;
typedef unsigned long long ll;

ll judgePrimeList[] = {
    2,3,5,7,11,13,17,19,23,29,31,37
};

/// <summary>
/// 기능 ; 두 수의 최대공약수 구하기
/// </summary>
ll GCD(ll x, ll y)
{
    if (x < y) swap(x, y);
    while (y != 0)
    {
        ll r = x % y;
        x = y;
        y = r;
    }
    return x;
}

/// <summary>
/// 기능 : a^m % p를 구하는 함수. 동작 방식은 위의 mult()와 동일하다.
/// </summary>
ll pow_mod(__int128 a, __int128 m, __int128 p)
{
    __int128 ret = 1;
    a %= p;

    while (m > 0)
    {
        if (m % 2 == 1) ret = (ret* a)% p;
        a = (a*a) % p;
        m /=2;
    }
    return (ll)ret;
}

/// <summary>
/// 기능 : 밀러-라빈 소수 판정
/// 조건 : 2이상의 수
/// </summary>
/// <param name="n">판정할 수</param>
bool MillerLavinJudgePrime(ll N, ll a)
{
    if (a % N == 0) return true;//N의 배수

    ll k = N - 1;//k = 2^h * m으로 나타낼 수 있다. 이때 h >= 1, m은 홀수다.

    while (true)
    {//a^{2^h * m}에서부터 거꾸로 진행한다.
        ll temp = pow_mod(a, k, N);//a^k가 통과하나?
        if (k % 2 == 1) return (temp == 1 || temp == N - 1);//만약 k가 홀수라면, a^m을 확인하고 있다는 말이다. 이 경우 a^m === 1 (mod n)인지를 확인한다.
        if (temp == N - 1) return true;//k가 짝수인 경우로, a^k === -1 (mod n)인 경우다.
        k /=2;//2로 나눠보자
    }
}
/// <summary>
/// 기능 : 소수 판별
/// </summary>
bool IsPrime(ll N)
{
    if (N == 2 || N == 3) return true;//2 or 3
    if (N % 2 == 0) return false;//짝수

    for (int i = 0; i < 12; i++)
    {
        if (!MillerLavinJudgePrime(N, judgePrimeList[i])) {//밀러-라빈 검사를 통과하지 못한 경우 합성수다.
            return false;
            break;
        }
    }
    return true;
}
/// <summary>
/// 기능 : 폴라-로 알고리즘을 이용한 소인수 분해
/// </summary>
/// <param name="N"></param>
ll PollarRhoFunction(__int128 N)
{
    //짝수
    if (N % 2 == 0) return 2;
    //소수
    if (IsPrime(N)) return N;

    __int128 x, y, c, d;
    do {
        // x, y, c를 새로 생성
        x = rand() % (N - 2) + 2;
        y = x;
        c = rand() % 10 + 1;
        d = 1;

        while (d == 1)
        {
            x = ((x * x% N) + c + N) % N;
            y = ((y * y% N) + c + N) % N;
            y = ((y * y% N) + c + N) % N;
            //if (y > x) swap(x, y); 시간 초과 원인
            d = GCD(abs(x - y), N);
        }
    } while (d == N); // d가 N이면 다시 루프를 반복

    if (IsPrime(d)) return d;
    return PollarRhoFunction(d);
}
/// <summary>
/// 기능 : 소인수분해 결과
/// 본 클래스의 메인 코드 -> 여기서 소인수 분해 결과가 나온다.
/// </summary>
vector<ll> FactorizationPrimes(ll N)
{
    vector<ll> factors;//소인수
    while (N > 1)
    {
        ll divisor = PollarRhoFunction(N);//소인수 구하기
        factors.push_back(divisor);
        N = N / divisor;
    }
    sort(factors.begin(), factors.end());//정렬
    return factors;
}

set<ll> UniqueFactor(vector<ll> factorizationPrimesList){
    set<ll> uniqueFactor;
    for(int i=0;i<factorizationPrimesList.size();i++){
        uniqueFactor.insert(factorizationPrimesList[i]);
    }
    return uniqueFactor;
}

int main() {
    //빠른 입력
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    ll n;
    cin >> n;
    
    //예외처리
    if(n==1){
        cout<<1;
        return 0;
    }
    
    vector<ll> factorizationPrimesList = FactorizationPrimes(n);//소인수 분해 진행
    //set<ll> factorizationPrimesSet = UniqueFactor(factorizationPrimesList);//고유 소인수들만 모음
    
    ll cnt = n;
    cnt = cnt/factorizationPrimesList[0]*(factorizationPrimesList[0]-1);
    for(int i=1; i<factorizationPrimesList.size(); i++)
        if(factorizationPrimesList[i] != factorizationPrimesList[i-1]) cnt = cnt/factorizationPrimesList[i]*(factorizationPrimesList[i]-1);
    cout << cnt;
    
    
    return 0;
}

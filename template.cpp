// cat `find . -name "[0-9][0-9]" | sort` > rael_in.txt
// cat `find . -name "[0-9][0-9].a" | sort` > rael_ou.txt
// g++ -o main codigo.cpp -Wall -Wextra -Wshadow -O2

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <climits>
#include <cctype>
#include <cmath>
#include <cassert>
#include <ctime>

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <deque>
#include <list>
#include <queue>
#include <stack>
#include <map>
#include <set>
#include <bitset>
//#include <complex>
#include <limits>
#include <functional>
#include <numeric>

#define iter(c) __typeof((c).begin())
#define Tr(a,b) for(iter(b) a = (b).begin(); a != (b).end(); ++a)
#define rF(a,b,c) for(int a = (b); a >= (c); --a)
#define Fr(a,b,c) for(int a = (b); a < (c); ++a)
#define Rp(a,c) Fr(a,0,c)
#define CL(a,b) memset(a, b, sizeof(a))
#define SZ(a) ((int) (a).size())
#define PB push_back
#define MP make_pair
#define F first
#define S second
#define oo 0x3F3F3F3F
#define LINF 0x3F3F3F3F3F3F3F3FLL

#define DBG(x) cout << #x << " == " << x << endl

using namespace std;

typedef long long ll;
typedef unsigned int uint;
typedef unsigned long long ull;
typedef unsigned long long rash;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
typedef vector<int> vi;
typedef vector<ll> vll;
typedef map<int,int> mii;

#define EPS 1e-7
#define dbg if(0)

int n;
int main() {
	while(scanf("%d", &n) == 1) {
		if(!n) break;
	}
	
	return 0;
}


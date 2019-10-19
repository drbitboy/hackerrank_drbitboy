#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <algorithm>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <cstring>
#include <string>
#include <ctime>
#include <queue>
#include <stack>
#include <vector>
#include <map>
#include <set>
#include <deque>
#include <cassert>
#include <unordered_map>
#include <bitset>
#include <unordered_set>

using namespace std;

#define pb push_back
#define pp pop_back
#define f first
#define s second
#define mp make_pair
#define sz(a) (int)((a).size())
#ifdef _WIN32
#  define I64 "%I64d"
#else
#  define I64 "%lld"
#endif
#define fname "."

typedef long long ll;
typedef unsigned long long ull;
typedef long double ld;
typedef pair < int, int > pi;
typedef pair < int, ull > pu;
typedef pair < ll, ll > pl;

const int inf = (int)1e9 + 123;
const int MAX_N = (int)2e5 + 123;

int n;
int w[MAX_N], b[MAX_N];

int main() {
#ifdef DEBUG
    freopen("input.txt", "r", stdin);
#endif
    scanf("%d", &n);
    for (int i = 1; i <= n; i++)
        scanf("%d", &w[i]);
    for (int i = 1; i <= n; i++)
        scanf("%d", &b[i]);
    
    set < int > pos;
    for (int i = 1; i <= n; i++) {
        pos.insert(n - i);
    }
    
    ll ans = 0;
    vector < pi > all;
    for (int i = 1; i <= n; i++) {
        all.pb(mp(w[i], b[i]));
    }
    sort(all.begin(), all.end());
    reverse(all.begin(), all.end());
    
    for (auto it : all) {
        auto xx = pos.lower_bound(it.s);
        if (xx == pos.end()) {
            ans += it.f;
        } else {
            pos.erase(xx);
        }
    }
    cout << ans << endl;
    return 0;
}

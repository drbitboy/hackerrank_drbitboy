///////////////////////////////////////////////////////////////////////
// Usage (BASH):
//
//     ./editor_btc [[< ]inputNN.txt] [--test=0]
//
// Example:
//
//     for i in input/input??.txt ; do ./editor_btc $i --test=3 ; done > x.x
//     cat output/output??.txt | diff x.x - | grep -q . || echo Success
//
///////////////////////////////////////////////////////////////////////


#include <bits/stdc++.h>
using namespace std;


typedef pair < int, int > PII;
typedef vector<PII> VPII;
typedef struct RESULTstr {
  int n;
  long long ans;
  VPII goods;
  VPII bads;
} RESULT, *pRESULT;


////////////////////////////////////////////////////////////////////////
RESULT& solve(int n, VPII& all, bool do_sort = true) {
pRESULT result = new RESULT;

    result->n = n;
    result->ans = 0L;
    result->goods.clear();
    result->bads.clear();

    set<int> pos;

#   if 0
    for (int i = 1; i <= result->n; i++) { pos.insert(result->n - i); }
#   else
    for (int i = 1; i < result->n; i++) { pos.insert(i); }
#   endif

    if (do_sort) {
        sort(all.begin(), all.end());
        reverse(all.begin(), all.end());
    }

    for (auto it : all) {
        auto xx = pos.lower_bound(it.second);
        if (xx == pos.end()) {
            result->ans += it.first;
            result->bads.push_back(it);
        } else {
            pos.erase(xx);
            result->goods.push_back(it);
        }
    }

    return *result;
}


////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
FILE* fin = stdin;
bool not_stdin = false;
int n_tests = 0;

    for (int iarg = 1; iarg<argc; ++iarg) {
    string sarg = string(argv[iarg]);

        // Command line argument [--test=N]
        if (!sarg.compare(0,7,"--test=")) {
            if (1 != sscanf(argv[iarg]+7, "%d", &n_tests)) {
                fprintf(stderr, "### Warning:  bad argument %d [%s]\n", iarg, argv[iarg]);
                fflush(stderr);
                n_tests = 0;
            }
            continue;
        }
        if (not_stdin) fclose(fin);
        fin = fopen(argv[iarg],"r");
        not_stdin = true;
    }
    int n; fscanf(fin, "%d", &n);
    int w[n+1]; for (int i = 1; i <= n; i++) fscanf(fin, "%d", &w[i]);
    char c = '\0'; while (c != '\n') fscanf(fin, "%c", &c);
    VPII all;
    for (int i = 1; i <= n; i++) {
        int b;
        fscanf(fin, "%d", &b);
        all.push_back(make_pair(w[i], b));
    }
    RESULT result = solve(n, all);
    cout << result.ans << endl;

    if (0 < n_tests) return 0;

    ////////////////////////////////////////////////////////////////////
    // Cf. https://www.hackerrank.com/contests/hackerrank-women-technologists-codesprint-2019/challenges/sound-system-testing/editorial
    // Test solver with random ordering of vector result.goods

    default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());
    // Number of tests is specified on command line via [--test=N] argument
    while (n_tests-- > 0) {
        // Shuffle goods vector, for which calculated weight is zero, from result
        shuffle(result.goods.begin(), result.goods.end(), generator);
        // Solver should return zero with shuffled vector
        // - [, false] => bypasses sort
        assert(!solve(result.n,result.goods,false).ans);
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////
//
// Solver for Hackerrank [Sound System Testing] cf.
//
//   https://www.hackerrank.com/contests/hackerrank-women-technologists-codesprint-2019/challenges/sound-system-testing
//
// Usage (BASH):
//
//     ./editor_btc [[< ]inputNN.txt] [--test=0]
//
//     - Files inputNN.txt are available from Hackerrank site
//
// Example:
//
//     for i in input/input??.txt ; do ./editor_btc $i --test=3 ; done > x.x
//     cat output/output??.txt | diff x.x - && echo Success || echo Failure
//
///////////////////////////////////////////////////////////////////////


#include <bits/stdc++.h>
using namespace std;


typedef pair < int, int > PII;
typedef vector<PII> VPII;

typedef struct RESULTstr {   // Result returned by solve(...) below
  int n;
  long long ans;
  VPII goods;
  VPII bads;
} RESULT, *pRESULT;


////////////////////////////////////////////////////////////////////////
// Solver Sound System Testing
// - N.B. for test, speakers.size() <= n && do_sort==false
RESULT& solve(int n, VPII& speakers, bool do_sort = true) {

pRESULT result = new RESULT;  // Instantiate and initialize result
    result->n = n;
    result->ans = 0L;
    result->goods.clear();
    result->bads.clear();

    if (speakers.empty()) return *result;  // Return zero if goods vector is empty

    // Instantiate and initialize set of available positions
    set<int> pos;
    for (int i = 0; i < result->n; i++) { pos.insert(i); }

    // Sort **only if requested**; e.g. no sort for testing
    if (do_sort) {
        // Sort input array of <sound strength,threshold position> pairs
        sort(speakers.begin(), speakers.end());
        reverse(speakers.begin(), speakers.end());
    }

    // Perform algorithm described by Birjik on Hackerrank site
    for (auto it : speakers) {
        auto xx = pos.lower_bound(it.second);
        if (xx == pos.end()) {
            result->ans += it.first;     // This speaker will contribute non-zero sound
            result->bads.push_back(it);  // Add it to bads vector
        } else {
            pos.erase(xx);               // This speacker will contribute no sound
            result->goods.push_back(it); // Add it to bads vector
        }
    }

    return *result;
}


////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) {
FILE* fin = stdin;
bool not_stdin = false;
int n_tests = 0;

    for (int iarg = 1; iarg<argc; ++iarg) {   // Process command line
    string sarg = string(argv[iarg]);

        if (!sarg.compare(0,7,"--test=")) {   // Command line argument [--test=N]

            if (1 != sscanf(argv[iarg]+7, "%d", &n_tests)) {
                fprintf(stderr, "### Warning:  bad argument %d [%s]\n", iarg, argv[iarg]);
                fflush(stderr);
                n_tests = 0;
            }
            continue;
        }

        if (not_stdin) fclose(fin);  // Close previously opened file, if not stin
        fin = fopen(argv[iarg],"r"); // Open file for reading
        not_stdin = true;            // Set flag that opened file is not stdin
    }

    int n; fscanf(fin, "%d", &n);          // Read N, size of problem

    int sound_strengths[n+1];              // Instantiate and read sound strengths (weights)
    for (int i = 1; i <= n; i++) {
        fscanf(fin, "%d", &sound_strengths[i]);
    }

    char c = '\0'; while (c != '\n') fscanf(fin, "%c", &c);  // Read to end of current line

    VPII speakers;  // Instantiate speakers as vector of <strength,threshold> pairs

    for (int i = 1; i <= n; i++) {  // Read threshold positions ...
        int b;
        fscanf(fin, "%d", &b);
        speakers.push_back(make_pair(sound_strengths[i], b));  // .. add pair to vector
    }

    RESULT result = solve(n, speakers);  // Get solution
    cout << result.ans << endl;          // Output answer, total sound

    if (0 < n_tests) return 0;           // Exit if testing was not specified

    ////////////////////////////////////////////////////////////////////
    // Cf. https://www.hackerrank.com/contests/hackerrank-women-technologists-codesprint-2019/challenges/sound-system-testing/editorial
    // Test solver with random ordering of vector result.goods

    default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

    // Number of tests is specified on command line via [--test=N] argument
    while (n_tests-- > 0) {

        // Shuffle goods vector, for which calculated weight is zero, from result
        shuffle(result.goods.begin(), result.goods.end(), generator);

        // Solver must return zero .ans[wer] and empty .bads vector
        // with shuffled goods vector input
        // - [, false] => bypasses sort
        assert(!solve(result.n,result.goods,false).ans && result.bads.empty());
    }
    return 0;
}

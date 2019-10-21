#include <bits/stdc++.h>

using namespace std;

static bool do_debug = getenv("BTCDEBUG") ? true : false;

typedef vector<string> VS;
typedef VS::iterator VSIT;

static VS the_vec;
static int L;
static int twoL;

////////////////////////////////////////////////////////////////////////
// Apppend current string, plus its reverse, to VS the_vec
////////////////////////////////////////////////////////////////////////
void add_to_the_vec(char* sofar) {
  string ssofar(sofar, twoL);
  the_vec.push_back(ssofar);
  the_vec.push_back(string(ssofar.rbegin(),ssofar.rend()));
}

////////////////////////////////////////////////////////////////////////
// Procedure [recurse]:  Visit remaining positions in grid
//
// - Append to string in buffer, which string starts at sofar, from a
//   position to the left of s1
//
// - The invariant state comprises one of two cases
//
// Invariant
// =========
//
//   (|s2|-1) <= |s1| <= |s2|
//
// Invariant case 1
// ================
//
//   s1 and s2 have equal lengths remaining, i.e. |s1|==|s2|, and
//
//     ...xxx*<s1------>
//     ...xxxx<s2------>
//
// where:
//
//     <si------>     => string s1 or s2
//     *              => current position in grid
//     ...xxx         => previous positions visited
//                       - order is stored, but otherwise irrelevant
//
//
// One of two alternate action are possible:
//
//   a) If |s1|,|s2| > 0, then step into s1 string and recurse with
//                             equal_lengths set to false
//
//   b) If |s1|,|s2| == 0, then pass stored data to add_to_vec procedure
//
//
// Invariant case 2
// ================
//
//   s1 and s2 have different lengths remaining, i.e. |s1|<|s2|,
//   specifically, |s1| = (|s2| - 1)
//
//     ...xxxx*<s1----->
//     ...xxxx<s2------>
//
//
// The two actions are
//
//   a) Continue stepping into s1, which means we must then continue
//      until all of s1 is used up, then step back through all of s2,
//      then pass stored data to add_to_vec procedure
//
//   b) Step down to s2 and recurse with equal_lengths set to true
//
////////////////////////////////////////////////////////////////////////

void recurse(char* sofar, char* sofarnext
            , char* s1, char* s2
            , bool equal_lengths
            ) {

  if (equal_lengths) {        // |s1|==|s2|:  s1 & s2 lengths are equal

    if (!*s1) {               // - End of s1,s2 strings; |s1|,|s2| == 0

      add_to_the_vec(sofar);  //   - Case 1.a add result to the vector

    } else {                     // - |s1|,|s2| > 0; Case 1.b

      *(sofarnext++) = *(s1++);  // - Add 1st character of s1 to buffer

      // Recurse with unequal lengths; N.B. shorter s1 arg is before s2

      recurse(sofar, sofarnext, s1, s2, false);

    }

  } else {                         // (|s1| + 1) == |s2|

    // Case 2.a

    char* ps1 = s1;                // Pointer to start of s1
    char* ps2end = s2;             // Pointer to start of s2; see below
    char* psofarnext = sofarnext;  // Local sofarnext pointer

    while (*ps1) {                 // Step into s1 until terminator
      *(psofarnext++) = *(ps1++);  // Copy remaining s1 to buffer
      ++ps2end;                    // Advance s2 end pointer
    }                              // On exit, *ps2end is last s2 char

    while (ps2end >= s2) *(psofarnext++) = *(ps2end--);  // copy s2 rev,
    add_to_the_vec(sofar);                               // and add sofar

    // Case 2.b

    *(sofarnext++) = *(s2++);  // Case 2.b add 1st s2 char to buffer

    // Recurse with equal lengths; N.B. s2 arg is before s1

    recurse(sofar, sofarnext, s2, s1, true);

  }
  return;
}

void gridlandProvinces_sub(char* s1, char* s2) {

  char buffer[(3*L) + 1];

  char* sofar = buffer + L;
  char* sofarnext = buffer + L;
  char* ps1 = s1;
  char* ps2 = s2;

  while (sofar > buffer) {
    *(--sofar) = *(ps1++);     // Copy s1 to buffer backward from middle
    *(sofarnext++) = *(ps2++); // Copy s2 to buffer forward from middle
  }

  //*sofarnext = '\0';

  while (sofarnext >= sofar) {
    recurse(sofar++, sofarnext--, ps2--, ps1--, true);
  }
}

int gridlandProvinces(string s1, string s2) {
  the_vec.reserve(723604);
  the_vec.clear();
  L = s1.length();
  twoL = L << 1;
  gridlandProvinces_sub((char*)s1.c_str(), (char*)s2.c_str());
  gridlandProvinces_sub((char*)s2.c_str(), (char*)s1.c_str());
  sort(the_vec.begin(), the_vec.end());
  VSIT lastit = the_vec.begin();
  VSIT tvend = the_vec.end();
  int glcount = 1;
  if (do_debug) {
    cout <<"[" <<the_vec.size() <<"]=the_vec.size()\n"; flush(cout);
  }
  for (VSIT vsit = the_vec.begin() + 1; vsit != tvend; ++vsit) {
    if (do_debug) {
      cout <<"[" << (*vsit) <<"]=vsit;[" << (*lastit) << "]=lastit;[" << glcount << "]=glcount\n"; flush(cout);
    }
    if (*vsit > *lastit) {
      ++glcount;
      lastit = vsit;
    }
  }
  return glcount;
}

int main(int argc, char** argv)
{
    ofstream fout(getenv("OUTPUT_PATH"));

    streambuf *cinbuf = 0;
    ifstream in;

    if (argc>1) {
      in.open(argv[1]);
      cinbuf = cin.rdbuf();
      cin.rdbuf(in.rdbuf());
    }
    //istream fin = argc<2 ? ifstream("/dev/stdin") : ifstream(argv[1]);

    int p;
    cin >> p;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    for (int p_itr = 0; p_itr < p; p_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string s1;
        getline(cin, s1);

        string s2;
        getline(cin, s2);

        fout << gridlandProvinces(s1, s2) << "\n";
    }

    if (cinbuf) cin.rdbuf(cinbuf);

    fout.close();

    return 0;
}

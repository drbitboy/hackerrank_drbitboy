#include <bits/stdc++.h>

using namespace std;

static bool do_debug = getenv("BTCDEBUG") ? true : false;

typedef set<string> SS;
typedef SS::iterator SSIT;
typedef vector<string> VS;
typedef VS::iterator VSIT;
typedef map<char*,set<string>> MP;
typedef MP::iterator MPIT;
typedef pair<char*,string> PPCS;

class Gridland {
public:
  MP warehouse;
  VS the_vec;
  int L;
  int twoL;

  Gridland() { }
  ~Gridland() { }

  //////////////////////////////////////////////////////////////////////
  // Append current string, plus its reverse, to VS the_vec
  //////////////////////////////////////////////////////////////////////
  void add_to_the_vec(string ssofar) {
    the_vec.push_back(ssofar);
    if (do_debug) cout << "[" << the_vec[the_vec.size()-1] << "]" << the_vec.size();
    the_vec.push_back(string(ssofar.rbegin(),ssofar.rend()));
    if (do_debug) cout << "[" << the_vec[the_vec.size()-1] << "]" << the_vec.size() << "\n";
  }

  void sub_solve(string* pss1, string* pss2) {
    char* s1 = (char*) pss1->c_str();
    char* s2 = (char*) pss2->c_str();
    char* ps1 = s1 + L;
    char* ps2 = s2 + L;

    string loop12 = (*pss2) + string(pss1->rbegin(), pss1->rend()) + (*pss2);
    string loop21 = (*pss1) + string(pss2->rbegin(), pss2->rend()) + (*pss1);

    char* ploop12 = L + (char*) loop12.c_str();
    char* ploop21 = L + (char*) loop21.c_str();
    char* ploop12_rev = ploop21;
    char* ploop21_rev = ploop12;

    MPIT whit1;
    MPIT whit2;
    MPIT new_whit1;
    MPIT new_whit2;

    if ((whit1 = warehouse.find(ps1)) == warehouse.end()) {
      warehouse[ps1] = SS();
      warehouse[ps2] = SS();
      warehouse[ps1].insert(string(""));
      warehouse[ps2].insert(string(""));
    }
    whit1 = warehouse.find(ps1);
    whit2 = warehouse.find(ps2);

    int n_left = twoL;
    int n_left_rev = 0;

    while (ps1 > s1) {
      char* prevps1 = ps1--;
      char* prevps2 = ps2--;
      char c3[3] = { *ps1, *ps2, *ps1 };
      SSIT prevv1beg = whit1->second.begin();
      SSIT prevv1end = whit1->second.end();
      SSIT prevv2beg = whit2->second.begin();
      SSIT prevv2end = whit2->second.end();

      new_whit1 = warehouse.find(ps1);
      new_whit2 = warehouse.find(ps2);
      bool new_whit = new_whit1 == warehouse.end();

      SS set1 = SS(); set1.clear();
      SS set2 = SS(); set2.clear();

      for (SSIT it=prevv1beg; it != prevv1end; ++it ) {
        add_to_the_vec(string(ploop21,n_left) + *it);
        if (new_whit) {
          set2.insert(string(c3+1,2) + *it);
          if (do_debug) cout << "[" << (string(c3+1,2) + *it) << "]=>set2\n";
        }
      }

      for (SSIT it=prevv2beg; it!= prevv2end; ++it ) {
        add_to_the_vec(string(ploop12,n_left) + *it);
        if (new_whit) {
          set1.insert(string(c3,2) + *it);
          if (do_debug) cout << "[" << (string(c3,2) + *it) << "]=>set1\n";
        }
      }

      if (new_whit) {

        n_left_rev += 2;
        --ploop12_rev;
        --ploop21_rev;

        set2.insert(string(ploop21_rev, n_left_rev));
        if (do_debug) cout << "[" << string(ploop21_rev, n_left_rev) << "]=>set2\n";

        set1.insert(string(ploop12_rev, n_left_rev));
        if (do_debug) cout << "[" << string(ploop12_rev, n_left_rev) << "]=>set1\n";

        warehouse[ps1] = set1;
        warehouse[ps2] = set2;
        whit1 = warehouse.find(ps1);
        whit2 = warehouse.find(ps2);
      } else {
        whit1 = new_whit1;
        whit2 = new_whit2;
      }

      ++ploop12;
      ++ploop21;
      n_left -= 2;
    }
#   if 1
    for (SSIT it=whit1->second.begin(); it != whit1->second.end(); ++it ) {
      add_to_the_vec(*it);
    }

    for (SSIT it=whit2->second.begin(); it != whit2->second.end(); ++it ) {
      add_to_the_vec(*it);
    }
#   endif

  } // Gridland.sub_solve(...)

  int solve(string& s1, string& s2) {
    the_vec.reserve(723604);
    the_vec.clear();
    warehouse.clear();
    L = s1.length();
    twoL = L << 1;
    sub_solve(&s1, &s2);
    //sub_solve(&s2, &s1);

    sort(the_vec.begin(), the_vec.end());
    VSIT lastit = the_vec.begin();
    VSIT tvend = the_vec.end();
    int glcount = 1;
    for (VSIT vsit = the_vec.begin() + 1; vsit != tvend; ++vsit) {
      if (do_debug) {
        cout << "[" << *lastit << "]=lastit;[";
      }
      if (*vsit > *lastit) {
        ++glcount;
        lastit = vsit;
      }
      if (do_debug) {
        cout << *vsit << "]=vsit;[" << glcount << "]=glcount\n";
      }
    }
    return glcount;
  } // int Gridland.solve(...)

}; // class Gridland

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

    Gridland gridland;

    for (int p_itr = 0; p_itr < p; p_itr++) {
        int n;
        cin >> n;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string s1;
        getline(cin, s1);

        string s2;
        getline(cin, s2);

        fout << gridland.solve(s1, s2) << "\n";
    }

    if (cinbuf) cin.rdbuf(cinbuf);

    fout.close();

    return 0;
}

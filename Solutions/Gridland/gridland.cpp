#include <bits/stdc++.h>

using namespace std;

static bool do_debug = getenv("BTCDEBUG") ? true : false;

#define chterm '\0'

long long compute_hash(char* pstring, size_t L, long long hash_value=0LL) {
const long long p = 31LL;
const long long m = 1000000009LL;
long long p_pow = 1;
  for (char* s = pstring+L; s-- > pstring; ) {
    hash_value = (hash_value + ((*s & 0x01f) * p_pow)) % m;
    p_pow = (p_pow * p) % m;
  }
  return hash_value;
}

class Htring {              // Hashed string
public:
  char* pstring;            // The string itself
  size_t L;                 // String length
  long long hash_value;

  Htring() : pstring(0), L(0), hash_value(0) { };

  Htring(const char* pin, size_t L_) : L(L_) {
    pstring = new char(L+1);
    memcpy(pstring, pin, L);
    pstring[L] = chterm;
    hash_value = compute_hash(pstring, L);
  };

  Htring(const Htring& huffix, const char* prefix = 0, size_t L_ = 0) {
    L = L_ + huffix.L;
    if (L) pstring = new char(L + 1);
    else   pstring = 0;
    if (L_) memcpy(pstring, prefix, L_);
    if (huffix.L) { memcpy(pstring+L_, huffix.pstring, huffix.L); }
    if (L) pstring[L] = chterm;
    hash_value = compute_hash(pstring, L_, huffix.hash_value);
  }

  ~Htring() { if (pstring) { delete[] pstring; } }

  Htring& operator=(const Htring& htr) {
    L = htr.L;
    hash_value = htr.hash_value;
    if (L) {
      pstring = new char(L+1);
      pstring[L] = chterm;
    } else {
      pstring = 0;
    }
    return *this;
  }
};

struct HtringCompare {
  bool operator()(const Htring left, const Htring right) {
    if (left.hash_value < right.hash_value) return true;
    if (left.hash_value > right.hash_value) return false;

    // To here, hashes are the same; compare strings
    // - First check for empty strings

    if (!left.L && !right.L) return false;
    if (!left.L) return true;              // Shorter string sorts first
    if (!right.L) return false;            // Shorter string sorts first

    // Neither string is empty

    int maxL = left.L > right.L ? left.L : right.L;
    return strncmp(left.pstring, right.pstring, maxL) < 0; 
  }
};

typedef set<Htring, HtringCompare> Hset;
typedef Hset::iterator HsetIT;
#if 0
typedef set<string> SS;
typedef SS::iterator SSIT;
typedef vector<string> VS;
typedef VS::iterator VSIT;
#endif
typedef map<char*, Hset> MP;
typedef MP::iterator MPIT;

class Gridland {
public:
  MP warehouse;
  Hset the_set;
  int L;
  int twoL;

  Gridland() { }
  ~Gridland() { }

  //////////////////////////////////////////////////////////////////////
  // Append current string, plus its reverse, to Hset the_set
  //////////////////////////////////////////////////////////////////////
  void add_to_the_set(Htring ssofar) {
    the_set.insert(ssofar);
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
      warehouse[ps1] = Hset();
      warehouse[ps2] = Hset();
      warehouse[ps1].insert(Htring("", 0));
      warehouse[ps2].insert(Htring("", 0));
    }
    whit1 = warehouse.find(ps1);
    whit2 = warehouse.find(ps2);

    int n_left = twoL;
    int n_left_rev = 0;

    while (ps1 > s1) {
      char* prevps1 = ps1--;
      char* prevps2 = ps2--;
      char c3[3] = { *ps1, *ps2, *ps1 };
      HsetIT prevv1beg = whit1->second.begin();
      HsetIT prevv1end = whit1->second.end();
      HsetIT prevv2beg = whit2->second.begin();
      HsetIT prevv2end = whit2->second.end();

      new_whit1 = warehouse.find(ps1);
      new_whit2 = warehouse.find(ps2);
      bool new_whit = new_whit1 == warehouse.end();

      Hset set1 = Hset(); set1.clear();
      Hset set2 = Hset(); set2.clear();

      for (HsetIT it=prevv1beg; it != prevv1end; ++it ) {
        add_to_the_set(Htring(*it, ploop21, n_left));
        if (new_whit) {
          set2.insert(Htring(*it, c3+1, 2));
          //if (do_debug) cout << "[" << (string(c3+1, 2) + *it) << "]=>set2\n";
        }
      }

      for (HsetIT it=prevv2beg; it!= prevv2end; ++it ) {
        add_to_the_set(Htring(*it, ploop12, n_left));
        if (new_whit) {
          set1.insert(Htring(*it, c3, 2));
          //if (do_debug) cout << "[" << (string(c3, 2) + *it) << "]=>set1\n";
        }
      }

      if (new_whit) {

        n_left_rev += 2;
        --ploop12_rev;
        --ploop21_rev;

        set2.insert(Htring(ploop21_rev, n_left_rev));
        //if (do_debug) cout << "[" << string(ploop21_rev, n_left_rev) << "]=>set2\n";

        set1.insert(Htring(ploop12_rev, n_left_rev));
        //if (do_debug) cout << "[" << string(ploop12_rev, n_left_rev) << "]=>set1\n";

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

    for (HsetIT it=whit1->second.begin(); it != whit1->second.end(); ++it ) {
      add_to_the_set(*it);
    }

    for (HsetIT it=whit2->second.begin(); it != whit2->second.end(); ++it ) {
      add_to_the_set(*it);
    }
  } // Gridland.sub_solve(...)

  int solve(string& s1, string& s2) {
    //the_vec.reserve(723604);
    the_set.clear();
    warehouse.clear();
    L = s1.length();
    twoL = L << 1;
    sub_solve(&s1, &s2);

    int glcount = the_set.size();

    HsetIT thend = the_set.end();

    for (HsetIT it = the_set.begin(); it != thend; ++it) {
      //if (the_set.find(string(it->rbegin(), it->rend()).c_str(), it->length()) == thend) {
      //  ++glcount;
      //}
      ;
    }

    return glcount;

#   if 0
    sort(the_vec.begin(), the_vec.end());
    VSIT lastit = the_vec.begin();
    VSIT tvend = the_vec.end();
    int glcount = 1;
    for (VSIT vsit = the_vec.begin() + 1; vsit != tvend; ++vsit) {
      if (do_debug) {
        ;//cout << "[" << *lastit << "]=lastit;[";
      }
      if (*vsit > *lastit) {
        ++glcount;
        lastit = vsit;
      }
      if (do_debug) {
        ;//cout << *vsit << "]=vsit;[" << glcount << "]=glcount\n";
      }
    }
    return glcount;
#   endif
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

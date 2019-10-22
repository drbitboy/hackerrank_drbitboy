#include <bits/stdc++.h>

using namespace std;

static bool do_debug = getenv("BTCDEBUG") ? true : false;

#define chterm '\0'

typedef class Onec *pOnec, **ppOnec;

class Onec {
public:
  int bits;
  pOnec ponecs[26];
  Onec() { }
  ~Onec() { };
}; // class Onec;

class AggOnec {
public:
# if 1
  const int n_per_alloc = 10000000;
# else
  // Testing
  const int n_per_alloc = 10;
# endif
  vector<pOnec> vpo;
  pOnec pNext;
  pOnec pEnd;
  ~AggOnec() {
    for (int i; i<vpo.size(); ++i) { delete[] vpo[i]; }
  }
  AggOnec() {
    vpo.clear();
    vpo.reserve(100);
    add_10M();
  }
  void add_10M() {
    pNext = new Onec[n_per_alloc];
    vpo.push_back(pNext);
    pEnd = pNext + n_per_alloc;
  }
}; // class Onec

typedef class City *pCity, **ppCity;
class City {
public:
  pOnec ponec;
  int bit;
  char i5;
  City() { };
  ~City() { }
  void set_char(char c) { i5 = c - 'a' ; bit = 1<<i5; }
  bool set_ponec(pOnec pOnec_) {
    // Return true if Once already has this char
    ponec = ponec_;
    bool bool_return;
    if (! (bool_return = (ponec->bits & bit) ? true : false)) {
      ponec->bits |= bit;
    }
    return bool_return;
  }
}; // class City


class Gridland {
public:
  AggOnec aggonec;
  int L;
  int twoL;

  Gridland() : aggonec(AggOnec()) { }
  ~Gridland() { }

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
    L = s1.length();
    twoL = L << 1;
    sub_solve(&s1, &s2);

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

#include <bits/stdc++.h>
using namespace std;

#define VI vector<int>
#define VIIT VI::iterator
#define RVIIT VI::reverse_iterator

void abc(int i, int* pi) {
  char s99[99];
  sprintf(s99, "%09lx\n%09lx\n%09lx\n", (long)i, (long)*pi, ((long)i) - ((long)*pi));
  cout << s99;
  flush(cout);
}
int
main() {
  int i = 16;
  abc(i--, &i);

  VI vint = {1,2,3,4,5};

  assert(&vint.back() == &(*vint.rbegin()));
  assert(vint.back() == *vint.rbegin());

  VI::reverse_iterator rvii = vint.rbegin();
  VI::iterator vii;

  cout << *(rvii++) << "=*(rvii++)\n";
  cout << *(rvii++) << "=*(rvii++)\n";
  cout << *(rvii)   << "=*(rvii)\n";

  vii = vint.erase((++rvii).base());
  cout << "[executed vii = vint.erase((++rvii).base());]\n";

  cout << *(vii)    << "=*(vii)\n";
  cout << *(rvii)   << "=*(rvii)\n";
  cout << *(++rvii) << "=*(++rvii)\n";
  cout << *(--rvii) << "=*(--rvii)\n";
  cout << *(--rvii) << "=*(--rvii)\n";

  for (vii = vint.begin(); vii != vint.end(); ++vii) {
    cout << (vii==vint.begin() ? "" : " ") << *vii;
  }
  cout << " = vint before\n";

  vint.erase(vint.rend().base());
  cout << "[executed vint.erase(vint.rend().base());]\n";

  for (vii = vint.begin(); vii != vint.end(); ++vii) {
    cout << (vii==vint.begin() ? "" : " ") << *vii;
  }
  cout << " = vint after\n";

  typedef int *PINT, **PPINT;

  PINT pint = new int[2];
  PPINT ppint = new PINT[2];

  cout << (100 + (&pint[1] - &pint[0]))
       << "," << (((long)(pint+1)) - ((long)pint))
       << "," << (sizeof pint[0])
       << "\n";
  cout << (100 + (&ppint[1] - &ppint[0]))
       << "," << (((long)(ppint+1)) - ((long)ppint))
       << "," << (sizeof ppint[0])
       << "\n";
  
  cout << "done\n";
  return 0;
}

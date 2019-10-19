#include <bits/stdc++.h>
using namespace std;

static bool do_print = getenv("DO_PRINT") ? true : false;

/*
 * Complete the 'minSound' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY strengths
 *  2. INTEGER_ARRAY threshold_dist
 */

////////////////////////////////////////////////////////////////////////
class SPKR {  // Class describing one speaker
public:
  int str;    // Strength
  int thr;    // Threshold position
  int posn;   // Position
  int sound;  // Sound level heard at origin
  SPKR() {}
  // Set position = threshold + 1, but no more than N+1; return result
  int set_ptp1(int N) { return (posn = (thr < N ? thr : N) + 1); }
  void set_posn(int posn_) { posn = posn_; }
  long long calc_sound() { return posn > thr ? 0L : (long long)str; }
  void print14(ostream& out) {
    char s99[99];
    sprintf(s99,"|%-6d,%-7d", thr, str%10000000);
    out << s99;
  }
};
typedef SPKR *pSPKR, **ppSPKR;
typedef vector<pSPKR> vpSPKR;
typedef vector<vpSPKR> vvpSPKR;

////////////////////////////////////////////////////////////////////////
// Comparison function for sorting one vector<pSPKR> group (of many in vvpSPKR)
bool compar_pspkr(pSPKR p1, pSPKR p2) {
  if (p1->str < p2->str) return false;  // Lesser strength sorts last
  if (p1->str > p2->str) return true;
  // Equal strengths
  if (p1->thr > p2->thr) return false;  // Greater threshold position sorts last
  if (p1->thr < p2->thr) return true;
  // Equal thresholds and strengths:
  // - use pointers to break ties
  return (p1 > p2);                     // Greater pointer sorts first
}

////////////////////////////////////////////////////////////////////////
// Same comparison, for qsort'ing (ppSPKR)SPKRS.ppspkrlist
int qcompar_pspkr(const void* pp1, const void* pp2) {
  pSPKR p1 = *((ppSPKR)pp1);
  pSPKR p2 = *((ppSPKR)pp2);
  if (p1->str < p2->str) return 1;  // Lesser strength sorts last
  if (p1->str > p2->str) return -1;
  // Equal strengths
  if (p1->thr > p2->thr) return 1;  // Greater threshold position sorts last
  if (p1->thr < p2->thr) return -1;
  // Equal thresholds and strengths:
  // - use pointers to break ties
  if (p1 > p2) return -1;           // Greater pointer sorts first
  if (p1 < p2) return 1;
  return 0;                         // N.B. WSNBATGH
}

////////////////////////////////////////////////////////////////////////
class SPKRS {             // Class containing all speakers
public:
  pSPKR pspkrs;           // Pointer to original list of SPKRs
  pSPKR pspkrsend;        // Pointer to end of SPKR list
  ppSPKR ppspkrlist;      // Pointer to list of [pSPKRs i.e. pointers to SPKRs]
  ppSPKR ppspkrlistend;   // Pointer to end of that pSPKR list
  vvpSPKR vvpspkrgroups;  // Vector of pSPKR items at each position

  int N;
  SPKRS(int N_) : N(N_) {
    pspkrs = new SPKR[N];                     // List of SPKR items
    pspkrsend = pspkrs + N;                   // End of SPKR list
    ppspkrlist = new pSPKR[N];                // List of pSPKR items
    ppspkrlistend = ppspkrlist;               // End of pSPKR list
    vvpspkrgroups.resize(N+2);                // pSPKR items out to position [N+1]

    // Copy pSPKRs to list, which will be sorted later
    for (pSPKR p=pspkrs; p<pspkrsend; ++p) *(ppspkrlistend++) = p;
  }
  ~SPKRS() { delete[] pspkrs; delete[] ppspkrlist; }

  long long min_sound() {

    // Local pointers and an int
    pSPKR pspkr;
    pSPKR pspkr1;
    ppSPKR ppspkr;
    int iposn;

    vvpSPKR::iterator vvgiter;   // group vectors' iterator

    // N.B. We ignore position zero, even though vvpspkrgroups[0] is allocated above

    // Clear all group vectors (probably not necessary)
    for (vvgiter=vvpspkrgroups.begin(); vvgiter != vvpspkrgroups.end(); ++vvgiter) {
      if (vvgiter->size()) vvgiter->clear();
    }

    int first_multi = N + 2;  // Initialize position of left-most multi-pSPKR group
    int first_empty = 1;      // Initialize position of left-most empty group

    ppspkr = ppspkrlist;      // Initialize local ppSPKR pointer to start of list

    for (pspkr=pspkrs; pspkr<pspkrsend; ++pspkr) {

      // Store pSPKR pointer intoto ppSPKR list
      *(ppspkr++) = pspkr;

      // Set each SPKR position to one past its threshold position,
      // and then append that SPKR's pointer to that position's group vector
      vvpspkrgroups[iposn = pspkr->set_ptp1(N)].push_back(pspkr);

      // Optionally adjust position of left-most multi-pSPKR group on addition of second item
      if (iposn < first_multi && 2 == vvpspkrgroups[iposn].size()) first_multi = iposn;

      // If this was the position of the left-most empty group, find the position of the next one
      if (iposn == first_empty) { while (vvpspkrgroups[++first_empty].size() && first_empty < N) ; }
    }

    // Sort all-pSPKR list by decreasing strength, increasing threshold position,
    // and decreasing pointer - they will be moved to the left in that order
    qsort(ppspkrlist,N,sizeof(pSPKR),qcompar_pspkr);

    // Sort per-position pointer groups
    for (iposn = 0; iposn < (N+1); ++iposn) {
      if (vvpspkrgroups[iposn].size() > 1) {
        sort(vvpspkrgroups[iposn].begin(), vvpspkrgroups[iposn].end(),compar_pspkr);
      }
    }

    // Setup complete:
    // 1. Each pSPKR is in a group vector one position beyond its effective threshold position (see 1.2.1)
    //    1.1. So total sound is zero at present
    //         1.1. Though either every position may not be filled, or
    //         1.2. some positions may have multiple pSPKRs
    //    1.2. Pointers with (threshold position) > N are in group vector at position (N+1)
    //         1.2.1. Position N is effective threshold position fo such pointers
    //         1.2.2. Such pointers violate third constraint in problem description, but it does not matter
    // 2. first_multi is the position of the left-most (lowest-position) multi-pSPKR group vector
    // 3. first_empty is the position of the left-most empty group vector

    // Algorith initialization
    long long sound_total = 0;                                  // Initialize sound total per 1.1 above
    ppSPKR ppspkrlast = ppspkrlistend;                          // Store pointer past last item in ppspkrlist

    vpSPKR vnonzeros;                                           // Vector to save pointers which increase total sound
    vnonzeros.clear();

    // Run algorithm in loop; each loop pass shifts one pSPKR
    // to the empty slot, until first_empty is position (N+1)
    while (1) {
      long long delta_sound;                                    // Calculated sound at a new position

#     define vpempty vvpspkrgroups[first_empty]                 // Initialize search for first empty vector (.size()==0) of pointer group

      while (first_empty <= N && vpempty.size()) {              // While first_empty is N or less, and vector at position is not empty ...
        ++first_empty;                                          // ... Increment first_empty
      }
      if (do_print) {
        cout << "\n========================================================================\n"
             << first_empty << "=first_empty\n"
             << first_multi << "=first_multi\n"
             << (((long)ppspkrlast)-((long)ppspkrlist)) << "=(ppspkrlast-ppspkrlist)\n"
             ;
        printout(cout, ppspkrlast);
      }

      if (first_empty > N) break;                               // No empty positions are left; terminate algorithm

      // Algorithm i:  free
      // Shift an extra pointer to the right, to the empty
      // position from a lower position
      if (first_multi < first_empty) {                          // If multi-pSPKR vector is in a position before first_empty ...
        vvgiter = vvpspkrgroups.begin() + first_multi;          // First multi-pSPKR vector
        vpempty.push_back(pspkr = vvgiter->back());             // Copy last pointer in vector to empty vector, ...
        vvgiter->pop_back();                                    // ... and pop that pointer off multi-pSPKR vector
        pspkr->set_posn(first_empty);                           // Set the pointed-to-SPKR's position
        while (2 > vvpspkrgroups[first_multi].size()) {         // If the vector at first_empty no longer has multiple pointers ...
          if (N < ++first_multi) break;                         // ... increment first_multi to find the next one that does
        }
        if (do_print) {
          cout << "free\n";
          flush(cout);
        }
        continue;                                               // This was a right-shift, there is no change in sound total; continue
      }

      // Algorithm ii:  inevitable
      // Take pointer from position (N+1), which pointer
      // must end up at or below its threshold anyway.
      if (vvpspkrgroups[N+1].size()) {
        vpempty.push_back(pspkr = vvpspkrgroups[N+1].back());   // Copy last pointer in last vector to empty vector ...
        vvpspkrgroups[N+1].pop_back();                          // ... and pop that pointer off last vector
        pspkr->set_posn(first_empty);                           // Set the pointer position
        sound_total += (delta_sound = pspkr->calc_sound());     // Adjust total
        //vnonzeros.push_back(pspkr);                           // Do not save this pointer since it is inevitable
        if (do_print) {
          cout << "inevitable,+"
               << delta_sound
               << "," << sound_total
               << "," << pspkr->str
               << "," << pspkr->thr
               << "," << pspkr->posn
               << "\n";
          flush(cout);
        }
        continue;                                               // Continue
      }

      // Algorithm iii:  greedy
      // Use a pointer from end of ppspkrlist, which points
      // to the right-most, lowest-strength SPKR.
      while (ppspkrlast > ppspkrlist) {                         // Don't let last pointer go past start of ppspkrlist
        pspkr = *(--ppspkrlast);                                // Decrement pointer to last pointer, save result
        if (pspkr->posn > first_empty) break;                   // Don't use pointer unless its position is right of first empty
      }
      vvgiter = vvpspkrgroups.begin() + pspkr->posn;            // Use last item on this vector
      if (do_print) {
        cout << "greedy"
             << "," << vvgiter->size() << "=vvgiter->size()"
             << "," << pspkr->str << "=pspkr->str"
             << "," << pspkr->thr << "=pspkr->thr"
             << "," << pspkr->posn << "=pspkr->posn(before)"
             ;
        flush(cout);
      }
      if (pspkr!=vvgiter->back()) {
        cout << "\n";
        for (int ivvg=0; ivvg<vvgiter->size(); ++ivvg) {
          pSPKR p = vvgiter->at(ivvg);
          cout << "vvgiter->at(" << ivvg << ")="
             << "[" << p << "=p"
             << "," << p->str << "=p->str"
             << "," << p->thr << "=p->thr"
             << "," << p->posn << "=p->posn"
             << "]\n"
             ;

        }
        flush(cout);
        assert(pspkr==vvgiter->back());                         // Sanity check
      }
      vpempty.push_back(pspkr);                                 // Copy last pointer in vector to empty vector, ...
      vvgiter->pop_back();                                      // ... and pop that pointer off multi-pSPKR vector
      pspkr->set_posn(first_empty);                             // Set the pointer position
      sound_total += (delta_sound = pspkr->calc_sound());       // Adjust total
      if (delta_sound) vnonzeros.push_back(pspkr);              // Save this pointer if it increased total sound
      if (do_print) {
        cout << "," << pspkr->posn << "=pspkr->posn(after)"
             << ",+" << delta_sound << "=delta_sound"
             << "," << sound_total << "=sound_total"
             << "\n";
        flush(cout);
      }

    } // while (1) (algorithm)

    while (1 < vnonzeros.size()) {

      if (do_print) {
        vpSPKR::iterator vnzit;
        for (vnzit = vnonzeros.begin(); vnzit != vnonzeros.end(); ++vnzit) {
          (*vnzit)->print14(cout);
          cout << "\n";
        }
        cout << "========\n";
        flush(cout);
      }

      vpSPKR::reverse_iterator rvnzit;

      pspkr = *(rvnzit = vnonzeros.rbegin());    // Get the last pSPKR in non-zero list
      vnonzeros.erase((++rvnzit).base());        // Drop that last pSPKR from the non-zero list, and move iterator
      int prevposn = pspkr->posn;                // Save the current position (posn) of that last pSPKR
      pspkr->set_posn(vnonzeros[0]->posn);       // Set that last pSPKR's posn to that of the first non-zero pSPKR
      if (do_print) {
        cout << prevposn << "=prevposn\n";
        cout << pspkr->posn << "=currposn\n";
        flush(cout);
      }
      while (rvnzit != vnonzeros.rend()) {       // Continue moving down non-zero list
        int currposn = (pspkr = *rvnzit)->posn;  // Get the next pSPKR and save its posn
        if (prevposn > pspkr->thr) {             // If the new posn is beyond the threshold ...
          sound_total -= pspkr->calc_sound();    // ... then back out this pSPKR's contribution to the total ...
          vnonzeros.erase((++rvnzit).base());    // ... and remove this pSPKR from the non-zero list, and increment reverse iterator
        } else { rvnzit++; }                     // Else only increment iterator
        pspkr->set_posn(prevposn);               // Set the new posn
        prevposn = currposn;                     // Save what was the current posn
      }
    }

    if (do_print) {
      cout << sound_total << "=sound_total\n";
      flush(cout);
    }

    return sound_total;

    // Heh
    return N + ((N&1) ? N : 0);
  }

  // Printout for debugging
  void printout(ostream& out, ppSPKR ppspkrlast) {
    int NN = N<11 ? N+2 : 11;

    int ivprint1st = (false && N==199923 ? 199460 : (N+1)) - (NN-1) ;

    char s99[99];

# define nlf out<<"\n";
# define C out<<s99

    sprintf(s99,"%d=N; [thr,str,posn]",N); C; nlf;

    sprintf(s99,"r\\c "); C;
    for (int i=1; i<NN; ++i) {
      int Nmi = ivprint1st + i;
      sprintf(s99,"|%-6d,%-7d", Nmi, vvpspkrgroups[Nmi].size()); C;
    }
    sprintf(s99,"|%-14s", "greedy"); C;
    nlf;

    pSPKR ps;

    for (int row=0; row < (N<NN ? N : NN); ++row) {
      sprintf(s99,"%-4d", row); C;
      for (int i=1; i<NN; ++i) {
      int iout;
      int Nmi = ivprint1st + i;
        if (row < vvpspkrgroups[Nmi].size()) {
          vvpspkrgroups[Nmi][row]->print14(out);
        } else {
          sprintf(s99,"|%-14s", " "); C;
        }
      }
      ppspkrlist[row]->print14(out);
      if ((ppspkrlist+row) == ppspkrlast) {
        sprintf(s99,"%s", "<-"); C;
      }

      nlf;
    }
    flush(out);
  }
};
typedef SPKRS *pSPKRS;


//static bool loob = ios_base::sync_with_stdio(false);
//static void* ratsdiov = (void*) cin.tie(NULL);

int main(int argc, char** argv)
{
FILE* fin = stdin;

  if (argc > 1) {
    fin = fopen(argv[1],"r");
    assert(fin);
  }
  int N; fscanf(fin, "%d", &N);                // Read N

  SPKRS spkrs(N);                        // Instantiate one SPKRS

  // Read sound strengths; read threshold positions
  for (pSPKR p = spkrs.pspkrs; p<spkrs.pspkrsend; ++p) { fscanf(fin, "%d", &p->str); }
  char c = '\0';
  int nscanf = 1;
  while (c != '\n' && 1 == fscanf(fin, "%c",&c)) ;
  for (pSPKR p = spkrs.pspkrs; p<spkrs.pspkrsend; ++p) { fscanf(fin, "%d", &p->thr); }

  ofstream fout(getenv("OUTPUT_PATH"));  // Open output stream
  fout << spkrs.min_sound() << "\n";     // Calculate and write minimum sound
  fout.close();                          // Close stream

  return 0;
}

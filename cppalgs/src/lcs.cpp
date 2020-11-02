#include <string.h>
/* Length of the longest common substring for two strings.
 *
 * For i, w/ 0<=i<=n
 *
 *  B C D B C D A
 *  A B E C B A
 *
 *   AGCM
 *   BFCN
 *
 *  1: AGCM  BFCN
 *     ...^  ...^
 *
 *  2: AGCM  BFCN
 *     ...^  ...^
 *
 */
int _lcs(int Ac, char Av[], int Bc, char Bv[]) {
  if (Ac < 0 || Bc < 0)
  // We
  {
    return 0;
  }

  if (Av[Ac] == Bv[Bc]) {
    return 1 + _lcs(Ac - 1, Av, Bc - 1, Bv);
  } else {
    int Al = _lcs(Ac - 1, Av, Bc, Bv);
    int Bl = _lcs(Ac, Av, Bc - 1, Bv);
    return Al > Bl ? Al : Bl;
  }
}

int lcs(char Av[], char Bv[]) {
  return _lcs(strlen(Av) - 1, Av, strlen(Bv) - 1, Bv);
}
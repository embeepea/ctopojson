#include "mod.h"

// mathematical modulus operator (doesn't do weird things for negative i, like % does):
int mod(int i, int m) {
  int r = i % m;
  if (r < 0) { r += m; }
  return r;
}


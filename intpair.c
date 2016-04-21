#include <stdlib.h>
#include "intpair.h"

int intPairsEqual(IntPair *p1, IntPair *p2) {
  return ((p1->a == p2->a) && (p1->b == p1->b)
          ||
          (p1->a == p2->b) && (p1->b == p1->a));
}

IntPair *cloneIntPair(IntPair *p) {
  IntPair *newIntPair = (IntPair*)malloc(sizeof(IntPair));
  newIntPair->a = p->a;
  newIntPair->b = p->b;
  return newIntPair;
}

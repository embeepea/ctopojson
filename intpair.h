#ifndef INTPAIR_H_INCLUDED
#define INTPAIR_H_INCLUDED

typedef struct IntPair {
  int a, b;
} IntPair;

int intPairsEqual(IntPair *p1, IntPair *p2);
IntPair *cloneIntPair(IntPair *p);

#endif // INTPAIR_H_INCLUDED

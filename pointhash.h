#ifndef POINTHASH_H_INCLUDED
#define POINTHASH_H_INCLUDED

#include "point.h"

typedef struct PointHash {
  int M;         // declared size
  int count;     // number actually occupied
  Point **keys;
} PointHash;

PointHash *newPointHash(int M);
int getPointIndex(PointHash *pointHash, Point *p);

#endif // POINTHASH_H_INCLUDED

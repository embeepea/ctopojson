#ifndef RING_H_INCLUDED
#define RING_H_INCLUDED

#include "point.h"
#include "pointhash.h"
#include "intpair.h"
#include "intlist.h"
#include "arclist.h"

typedef struct Ring {
  int n;
  Point *points;
  int *junctions;
  int njunctions;
  ArcList *arcList;
  IntList *arcIndices;
} Ring;

void printRing(Ring *ring);
Ring *doublePointArrayToRing(int n, double *x, double *y);
void traverseForJunctions(Ring *ring, PointHash *pointHash, IntPair **pointNeighbors);
void dumpRing(Ring *ring);
void printRingArcs(Ring *ring);

IntPair *JUNCTION;

#endif // RING_H_INCLUDED

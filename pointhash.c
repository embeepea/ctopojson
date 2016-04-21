#include <stdlib.h>
#include <stdio.h>
#include "pointhash.h"
#include "simplehash.h"

static int hashCode(int a, int b) {
  return 31*a + b;
}

static int modularHash(Point *p, int M) {
  return (hashCode(p->x,p->y) & 0x7fffffff) % M;
}

SIMPLEHASH_IMPLEMENTATION(PointHash, Point, newPointHash, getPointIndex, modularHash, pointsEqual)

#include <stdlib.h>
#include <stdio.h>
#include "archash.h"
#include "simplehash.h"
#include "mod.h"

static int hashCode(Arc *a) {
  int i,j;
  int hash = 0;
  for (i=0,j=a->first; i<a->count; ++i,j=mod(j+a->dir,a->size)) {
    hash = 31*hash + a->points[j].x;
    hash = 31*hash + a->points[j].y;
  }
  return hash;
}

static int modularHash(Arc *a, int M) {
  return (hashCode(a) % 0x7fffffff) % M;
}

SIMPLEHASH_IMPLEMENTATION(ArcHash, Arc, newArcHash, getArcIndex, modularHash, arcsEqual)

int getReverseArcIndex(ArcHash *hash, Arc *a) {
  reverseArc(a);
  int i = getArcIndex(hash, a);
  reverseArc(a);
  return i;
}

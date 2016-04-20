#include <stdlib.h>
#include <stdio.h>
#include "pointhash.h"

/*
 * Bare bones implementation of a hash whose keys are Points and whose
 * values are integers.  You don't get to choose the value for a Point;
 * it's assigned automatically when the point is inserted.
 *
 * This hash does not support removing a key.
 *
 * Furthermore, `get` and `insert` are really the same operation,
 * since we'd want to insert any point being queried if it's not
 * already there, and inserting a Point returns the corresponding (int)
 * value associated with it.
 *
 * So, this implementation just provides a single method `getPointIndex`.
 */

PointHash *newPointHash(int M) {
  PointHash *pointHash = (PointHash*)malloc(sizeof(PointHash));
  pointHash->M = M;
  pointHash->keys = (Point**)malloc(sizeof(Point*)*M);
  int i;
  for (i=0; i<M; ++i) {
    pointHash->keys[i] = NULL;
  }
  pointHash->count = 0;
  return pointHash;
}

int hashCode(int a, int b) {
  return 31*a + b;
}

int modularHash(int a, int b, int M) {
  return (hashCode(a,b) & 0x7fffffff) % M;
}

int getPointIndex(PointHash *pointHash, Point *p) {
  // Like a combination of `get` and `insert`: inserts a Point p into a PointHash,
  // if it is not already in it, and return the integer associated with it
  int i = modularHash(p->x, p->y, pointHash->M);
  int n = 0;
  while (pointHash->keys[i] != NULL) {
    ++n;
    if (n >= pointHash->M) {
      fprintf(stderr, "Error: PointHash capacity exceeded\n");
      exit(-1);
    }
    if (pointsEqual(p, pointHash->keys[i])) { // found this point
      return i;
    }
    i = (i + 1) % pointHash->M;
  }
  pointHash->keys[i] = p;
  ++(pointHash->count);
  return i;
}

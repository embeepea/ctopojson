#include <stdio.h>
#include <stdlib.h>
#include "ring.h"
#include "pointhash.h"
#include "intpair.h"
#include "intlist.h"

void printRing(Ring *ring) {
  printf("Ring(%1d)[", ring->n);
  int i;
  for (i=0; i<ring->n; ++i) {
    if (i>0) { printf(","); }
    printPoint(&(ring->points[i]));
  }
  printf("]");
}

Ring *doublePointArrayToRing(int n, double *x, double *y) {
  int i, ix, iy;
  int m = 0;
  Ring *ring = (Ring*)malloc(sizeof(Ring));
  ring->junctions = NULL;
  ring->arcList = NULL;
  ring->arcIndices = newIntList(4);
  ring->njunctions = 0;
  ring->n = n;
  ring->points = (Point*)malloc(sizeof(Point)*n);
  for (i=0; i<n; ++i) {
    doubleXYToPoint(&(ring->points[m]), x[i], y[i]);
    if ((m == 0) || !pointsEqual(&(ring->points[m]), &(ring->points[m-1]))) {
      // eliminate consecutive identical points
      ++m;
    }
  }
  // if we got fewer than n distinct points, copy to new points array of correct length
  if (m < n) {
    Point *newPoints = (Point*)malloc(sizeof(Point)*m);
    for (i=0; i<m; ++i) {
      newPoints[i].x = ring->points[i].x;
      newPoints[i].y = ring->points[i].y;
    }
    free(ring->points);
    ring->points = newPoints;
    ring->n = m;
  }
  return ring;
}

IntPair junctionPair;
IntPair *JUNCTION = &junctionPair;

void traverseForJunctions(Ring *ring, PointHash *pointHash, IntPair **pointNeighbors) {
  int i, target;
  int N = ring->n;
  IntPair neighbors, *pNt;
  for (i=0; i<ring->n; ++i) {
    neighbors.a = getPointIndex(pointHash, &(ring->points[ i       ]));
    target      = getPointIndex(pointHash, &(ring->points[ (i+1)%N ]));
    neighbors.b = getPointIndex(pointHash, &(ring->points[ (i+2)%N ]));
    pNt = pointNeighbors[target];
    if (pNt != JUNCTION) {
      if (pNt == NULL) {
        pointNeighbors[target] = cloneIntPair(&neighbors);
      } else {
        if (intPairsEqual(pNt, &neighbors)) {
          pointNeighbors[target] = JUNCTION;
        }
      }
    }
  }
}

void setJunctions(Ring *ring, PointHash *pointHash, IntPair **pointNeighbors) {
  int i, j;
  ring->njunctions = 0;
  for (i=0; i<ring->n; ++i) {
    if (pointNeighbors[getPointIndex(pointHash, &(ring->points[i]))] == JUNCTION) {
      ++(ring->njunctions);
    }
  }
  if (ring->njunctions > 0) {
    ring->junctions = (int*)malloc(sizeof(int)*ring->njunctions);
    for (i=0, j=0; i<ring->n; ++i) {
      if (pointNeighbors[getPointIndex(pointHash, &(ring->points[i]))] == JUNCTION) {
        ring->junctions[j++] = i;
      }
    }
  }
}

void dumpRing(Ring *ring) {
  int i, j, isjunction;
  printf("Ring:\n");
  printf("           n: %1d\n", ring->n);
  if (ring->n > 0) {
    j = 0;
    for (i=0; i<ring->n; ++i) {
      if (j < ring->njunctions && i == ring->junctions[j]) {
        isjunction = 1;
        ++j;
      } else {
        isjunction = 0;
      }
      if (i==0) {
        printf("      points: %s[%4d] (%10d, %10d)\n",
               isjunction ? "* " : "  ",
               i, ring->points[i].x, ring->points[i].y
               );
      } else {
        printf("              %s[%4d] (%10d, %10d)\n",
               isjunction ? "* " : "  ",
               i, ring->points[i].x, ring->points[i].y
               );
      }
    }
  }
  printf("  njunctions: %1d\n", ring->njunctions);
  printf("   junctions: [");
  for (i=0; i<ring->njunctions; ++i) {
    if (i>0) { printf(", "); }
    printf("%1d", ring->junctions[i]);
  }
  printf("]\n");

  printf("        arcs: [");
  for (i=0; i<ring->arcIndices->count; ++i) {
    if (i>0) { printf(", "); }
    printf("%1d", ring->arcIndices->elements[i]);
  }
  printf("]\n");

}

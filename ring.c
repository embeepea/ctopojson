#include <stdio.h>
#include "ring.h"

void printRing(Ring *ring) {
  printf("Ring(%1d)[", ring->n);
  int i;
  for (i=0; i<ring->n; ++i) {
    if (i>0) { printf(","); }
    printPoint(&(ring->points[i]));
  }
  printf("]");
}

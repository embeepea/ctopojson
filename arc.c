#include <stdio.h>
#include "arc.h"
#include "point.h"

// mathematical modulus operator (doesn't do weird things for negative i, like % does):
int mod(int i, int m) {
  int r = i % m;
  if (r < 0) { r += m; }
  return r;
}

int arcsEqual(Arc *a1, Arc *a2) {
  if (a1->count != a2->count) { return 0; }
  int count = a1->count;

  int i1 = a1->first;
  int i2 = a2->first;
  int equal = 1;
  while (count) {
    if (!pointsEqual(&(a1->points[i1]), &(a2->points[i2]))) {
      equal = 0;
      break;
    }
    i1 = mod(i1 + 1, a1->size);
    i2 = mod(i2 + 1, a2->size);
  }
  if (equal) { return 1; }

  count = a1->count;
  i1 = a1->first;
  i2 = mod(a2->first + count-1, a2->size);
  equal = 1;
  while (count) {
    if (!pointsEqual(&(a1->points[i1]), &(a2->points[i2]))) {
      equal = 0;
      break;
    }
    i1 = mod(i1 + 1, a1->size);
    i2 = mod(i2 - 1, a2->size);
  }
  if (equal) { return -1; }

  return 0;
}

void dumpArc(Arc *a) {
  int i;
  printf("Arc:\n");
  printf("size(points):   %1d\n", a->size);
  printf("      points:   [%4d] (%10d, %10d)\n", 0, a->points[0].x, a->points[0].y);
  printf("                ...\n");
  printf("       first:   %1d\n", a->first);
  printf("       count:   %1d\n", a->count);
  printf("         arc:   [");
  for (i=0; i<a->count; ++i) {
    if (i>0) { printf(", "); }
    printf("%1d", (a->first + i) % a->size);
  }
  printf("]\n");
}
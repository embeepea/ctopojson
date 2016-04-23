#ifndef ARC_H_INCLUDED
#define ARC_H_INCLUDED

#include <stdio.h>
#include "point.h"

typedef struct Arc {
  Point *points;
  int size;  // declared length of points array
  int first; // index in points array of first point in this arc
  int count; // number of points in this arc
  int dir;   // traversal direction: 1 or -1
} Arc;

int arcsEqual(Arc *a1, Arc *a2);
void reverseArc(Arc *a);
void dumpArc(Arc *a);
void outputEncodedArc(FILE *fp, Arc *a);

#endif // ARC_H_INCLUDED

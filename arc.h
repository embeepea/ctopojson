#ifndef ARC_H_INCLUDED
#define ARC_H_INCLUDED

#include "point.h"

typedef struct Arc {
  Point *points;
  int size;  // declared length of points array
  int first; // index in points array of first point in this arc
  int count; // number of points in this arc
} Arc;

int arcsEqual(Arc *a1, Arc *a2);
void dumpArc(Arc *a);

#endif // ARC_H_INCLUDED

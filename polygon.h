#ifndef POLYGON_H_INCLUDED
#define POLYGON_H_INCLUDED

#include "intlist.h"

typedef struct Polygon {
  IntList *ringIndices;
} Polygon;

Polygon *newPolygon();

#endif // POLYGON_H_INCLUDED

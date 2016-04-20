#ifndef MULTIPOLYGON_H_INCLUDED
#define MULTIPOLYGON_H_INCLUDED

#include "polygonlist.h"

typedef struct MultiPolygon {
  PolygonList *polygonList;
} MultiPolygon;

MultiPolygon *newMultiPolygon();

#endif // MULTIPOLYGON_H_INCLUDED

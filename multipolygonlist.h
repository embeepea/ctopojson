#ifndef MULTIPOLYGONLIST_H_INCLUDED
#define MULTIPOLYGONLIST_H_INCLUDED

#include "multipolygon.h"

typedef struct MultiPolygonList {
  MultiPolygon **multiPolygons;
  int size;   // allocated size of multiPolygons array
  int count; // number of currently populated elements in multiPolygons array
} MultiPolygonList;

MultiPolygonList *newMultiPolygonList(int N);
void expandMultiPolygonList(MultiPolygonList *multiPolygonList);
int addMultiPolygon(MultiPolygonList *multiPolygonList, MultiPolygon *multiPolygon);

#endif // MULTIPOLYGONLIST_H_INCLUDED

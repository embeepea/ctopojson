#ifndef POLYGONLIST_H_INCLUDED
#define POLYGONLIST_H_INCLUDED

#include "polygon.h"

typedef struct PolygonList {
  Polygon **polygons;
  int size;   // allocated size of polygons array
  int count; // number of currently populated elements in polygons array
} PolygonList;

PolygonList *newPolygonList(int N);
void expandPolygonList(PolygonList *polygonList);
int addPolygon(PolygonList *polygonList, Polygon *polygon);

#endif // POLYGONLIST_H_INCLUDED

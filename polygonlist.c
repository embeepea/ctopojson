#include <stdlib.h>
#include "polygonlist.h"

PolygonList *newPolygonList(int N) {
  PolygonList *polygonList = (PolygonList*)malloc(sizeof(PolygonList));
  polygonList->size = N;
  polygonList->count = 0;
  polygonList->polygons = (Polygon**)malloc(sizeof(Polygon*)*N);
  return polygonList;
}

void expandPolygonList(PolygonList *polygonList) {
  int i, oldsize = polygonList->size;
  polygonList->size = 2*polygonList->size;
  Polygon **newPolygons = (Polygon**)malloc(sizeof(Polygon*)*polygonList->size);
  for (i=0; i<polygonList->count; ++i) {
    newPolygons[i] = polygonList->polygons[i];
  }
  free(polygonList->polygons);
  polygonList->polygons = newPolygons;
}

int addPolygon(PolygonList *polygonList, Polygon *polygon) {
  int i = polygonList->count;
  ++(polygonList->count);
  if (polygonList->count >= polygonList->size) {
    expandPolygonList(polygonList);
  }
  polygonList->polygons[i] = polygon;
  return i;
}

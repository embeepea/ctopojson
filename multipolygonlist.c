#include <stdlib.h>
#include "multipolygonlist.h"

MultiPolygonList *newMultiPolygonList(int N) {
  MultiPolygonList *multiPolygonList = (MultiPolygonList*)malloc(sizeof(MultiPolygonList));
  multiPolygonList->size = N;
  multiPolygonList->count = 0;
  multiPolygonList->multiPolygons = (MultiPolygon**)malloc(sizeof(MultiPolygon*)*N);
  return multiPolygonList;
}

void expandMultiPolygonList(MultiPolygonList *multiPolygonList) {
  int i, oldsize = multiPolygonList->size;
  multiPolygonList->size = 2*multiPolygonList->size;
  MultiPolygon **newMultiPolygons = (MultiPolygon**)malloc(sizeof(MultiPolygon*)*multiPolygonList->size);
  for (i=0; i<multiPolygonList->count; ++i) {
    newMultiPolygons[i] = multiPolygonList->multiPolygons[i];
  }
  free(multiPolygonList->multiPolygons);
  multiPolygonList->multiPolygons = newMultiPolygons;
}

int addMultiPolygon(MultiPolygonList *multiPolygonList, MultiPolygon *multiPolygon) {
  int i = multiPolygonList->count;
  ++(multiPolygonList->count);
  if (multiPolygonList->count >= multiPolygonList->size) {
    expandMultiPolygonList(multiPolygonList);
  }
  multiPolygonList->multiPolygons[i] = multiPolygon;
  return i;
}

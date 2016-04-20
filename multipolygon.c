#include <stdlib.h>
#include "multipolygon.h"

MultiPolygon *newMultiPolygon() {
  MultiPolygon *multiPolygon = (MultiPolygon*)malloc(sizeof(MultiPolygon));
  multiPolygon->polygonList = newPolygonList(1);
  return multiPolygon;
}

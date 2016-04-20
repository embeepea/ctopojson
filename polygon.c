#include <stdlib.h>
#include "polygon.h"

Polygon *newPolygon() {
  Polygon *polygon = (Polygon*)malloc(sizeof(Polygon));
  polygon->ringIndices = newIntList(1);
  return polygon;
}

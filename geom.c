#include <stdlib.h>
#include "geom.h"

Geom *newGeom(int type) {
  Geom *geom = (Geom*)malloc(sizeof(Geom));
  geom->type = type;
  if (type == GEOM_POLYGON) {
    geom->polygon = newPolygon();
  }
  if (type == GEOM_MULTIPOLYGON) {
    geom->multiPolygon = newMultiPolygon();
  }
  return geom;
}

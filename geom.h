#ifndef GEOM_H_INCLUDED
#define GEOM_H_INCLUDED

#include "polygon.h"
#include "multipolygon.h"

#define GEOM_POLYGON      1
#define GEOM_MULTIPOLYGON 2

typedef struct Geom {
  short unsigned int type;
  union {
    Polygon *polygon;
    MultiPolygon *multiPolygon;
  };
} Geom;

Geom *newGeom(int type);

#endif // GEOM_H_INCLUDED

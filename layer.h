#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED

#include "geomlist.h"
#include "polygonlist.h"
#include "multipolygon.h"
#include "multipolygonlist.h"

typedef struct Layer {
  GeomList *geomList;
} Layer;

Layer *newLayer();

#endif // LAYER_H_INCLUDED

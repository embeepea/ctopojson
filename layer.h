#ifndef LAYER_H_INCLUDED
#define LAYER_H_INCLUDED

#include "polygon.h"
#include "polygonlist.h"
#include "multipolygon.h"
#include "multipolygonlist.h"

typedef struct Layer {

  PolygonList *polygonList;
  MultiPolygonList *multiPolygonList;

} Layer;

Layer *newLayer();

#endif // LAYER_H_INCLUDED

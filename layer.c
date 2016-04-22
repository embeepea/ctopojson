#include <stdlib.h>
#include "layer.h"

Layer *newLayer() {
  Layer *layer = (Layer*)malloc(sizeof(Layer));
  layer->polygonList = newPolygonList(4);
  layer->multiPolygonList = newMultiPolygonList(4);
  return layer;
}

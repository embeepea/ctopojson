#include <stdlib.h>
#include "layer.h"

Layer *newLayer() {
  Layer *layer = (Layer*)malloc(sizeof(Layer));
  layer->geomList = newGeomList(4);
  return layer;
}

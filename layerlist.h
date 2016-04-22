#ifndef LAYERLIST_H_INCLUDED
#define LAYERLIST_H_INCLUDED

#include "layer.h"
#include "list.h"

LIST_DECLARATIONS(LayerList, Layer *, newLayerList, addLayer, expandLayerList)

#endif // LAYERLIST_H_INCLUDED

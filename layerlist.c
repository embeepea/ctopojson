#include <stdlib.h>
#include "layerlist.h"

LIST_IMPLEMENTATION(LayerList, Layer *, newLayerList, addLayer, expandLayerList)

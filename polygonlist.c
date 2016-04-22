#include <stdlib.h>
#include "polygonlist.h"
#include "list.h"

LIST_IMPLEMENTATION(PolygonList, Polygon *, newPolygonList, addPolygon, expandPolygonList)

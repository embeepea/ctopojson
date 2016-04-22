#include <stdlib.h>
#include "geomlist.h"

LIST_IMPLEMENTATION(GeomList, Geom *, newGeomList, addGeom, expandGeomList)

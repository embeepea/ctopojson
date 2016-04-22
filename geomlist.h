#ifndef GEOMLIST_H_INCLUDED
#define GEOMLIST_H_INCLUDED

#include "geom.h"
#include "list.h"

LIST_DECLARATIONS(GeomList, Geom *, newGeomList, addGeom, expandGeomList)

#endif // GEOMLIST_H_INCLUDED

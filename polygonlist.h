#ifndef POLYGONLIST_H_INCLUDED
#define POLYGONLIST_H_INCLUDED

#include "polygon.h"
#include "list.h"

LIST_DECLARATIONS(PolygonList, Polygon *, newPolygonList, addPolygon, expandPolygonList)

#endif // POLYGONLIST_H_INCLUDED

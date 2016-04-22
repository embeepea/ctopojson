#ifndef MULTIPOLYGONLIST_H_INCLUDED
#define MULTIPOLYGONLIST_H_INCLUDED

#include "multipolygon.h"
#include "list.h"

LIST_DECLARATIONS(MultiPolygonList, MultiPolygon *, newMultiPolygonList, addMultiPolygon, expandMultiPolygonList)

#endif // MULTIPOLYGONLIST_H_INCLUDED

#ifndef ARCLIST_H_INCLUDED
#define ARCLIST_H_INCLUDED

#include "arc.h"
#include "intpair.h"
#include "list.h"

LIST_DECLARATIONS(ArcList, Arc *, newArcList, addArc, expandArcList)

int arcLength(int i, int j, int size);

#endif // ARCLIST_H_INCLUDED

#ifndef ARCLIST_H_INCLUDED
#define ARCLIST_H_INCLUDED

#include "arc.h"
#include "ring.h"
#include "intpair.h"

typedef struct ArcList {
  Arc **arcs;
  int size;  // allocated size of arcs array
  int count; // number of currently populated elements in arcs array
} ArcList;

ArcList *ringToArcList(Ring *ring);

#endif // ARCLIST_H_INCLUDED

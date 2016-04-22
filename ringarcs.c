#include <stdlib.h>
#include "ringarcs.h"

ArcList *ringToArcList(Ring *ring) {
  ArcList *arcList = (ArcList*)malloc(sizeof(ArcList));
  if (ring->n <= 0) {
    // ring with no points should never happen, but if it does, return empty arc list
    arcList->size = 0;
    arcList->count = 0;
    arcList->elements = NULL;
    return;
  }
  arcList->size = ring->njunctions;
  if (ring->njunctions == 0) {
    arcList->size = 1;
  }
  arcList->count = arcList->size;
  arcList->elements = (Arc**)malloc(sizeof(Arc*)*(arcList->size));
  int i;
  Arc *arc;
  if (ring->njunctions > 0) {
    for (i=0; i<ring->njunctions; ++i) {
      arc = (Arc*)malloc(sizeof(Arc));
      arc->points = ring->points;
      arc->size = ring->n;
      arc->first = ring->junctions[i];
      arc->count = arcLength(ring->junctions[i], ring->junctions[(i+1)%(ring->njunctions)], arc->size);
      arc->dir = 1;
      arcList->elements[i] = arc;
    }
  } else {
    // here we have a ring -- no junctions
    // this is where we choose a rotation-invariant starting point
    arc = (Arc*)malloc(sizeof(Arc));
    arc->points = ring->points;
    arc->size = ring->n;
    arc->first = 0;
    for (i=1; i<ring->n; ++i) {
      if (pointsCompareLess(&(ring->points[i]), &(ring->points[arc->first]))) {
        arc->first = i;
      }
    }
    arc->count = ring->n + 1;
    arc->dir = 1;
    arcList->elements[0] = arc;
  }
 return arcList;
}


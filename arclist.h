#ifndef ARCLIST_H_INCLUDED
#define ARCLIST_H_INCLUDED

#include "arc.h"
#include "intpair.h"

typedef struct ArcList {
  Arc **arcs;
  int size;  // allocated size of arcs array
  int count; // number of currently populated elements in arcs array
} ArcList;


#endif // ARCLIST_H_INCLUDED

#include <stdlib.h>
#include "ringlist.h"

RingList *newRingList(int N) {
  RingList *ringList = (RingList*)malloc(sizeof(RingList));
  ringList->size = N;
  ringList->count = 0;
  ringList->elements = (Ring**)malloc(sizeof(Ring*)*N);
  return ringList;
}

void expandRingList(RingList *ringList) {
  int i, oldsize = ringList->size;
  ringList->size = 2*ringList->size;
  Ring **newRings = (Ring**)malloc(sizeof(Ring*)*ringList->size);
  for (i=0; i<ringList->count; ++i) {
    newRings[i] = ringList->elements[i];
  }
  free(ringList->elements);
  ringList->elements = newRings;
}

int addRing(RingList *ringList, Ring *ring) {
  int i = ringList->count;
  ++(ringList->count);
  if (ringList->count >= ringList->size) {
    expandRingList(ringList);
  }
  ringList->elements[i] = ring;
  return i;
}

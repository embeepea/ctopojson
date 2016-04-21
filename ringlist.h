#ifndef RINGLIST_H_INCLUDED
#define RINGLIST_H_INCLUDED

#include "ring.h"
#include "list.h"

LIST_DECLARATIONS(RingList, Ring *, newRingList, addRing, expandRingList)

// typedef struct RingList {
//   Ring **rings;
//   int size;   // allocated size of rings array
//   int count; // number of currently populated elements in rings array
// } RingList;
// 
// RingList *newRingList(int N);
// void expandRingList(RingList *ringList);
// int addRing(RingList *ringList, Ring *ring);

#endif // RINGLIST_H_INCLUDED

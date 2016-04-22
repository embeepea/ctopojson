#ifndef RINGLIST_H_INCLUDED
#define RINGLIST_H_INCLUDED

#include "ring.h"
#include "list.h"

LIST_DECLARATIONS(RingList, Ring *, newRingList, addRing, expandRingList)

#endif // RINGLIST_H_INCLUDED

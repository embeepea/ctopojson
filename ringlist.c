#include <stdlib.h>
#include "ringlist.h"

LIST_IMPLEMENTATION(RingList, Ring *, newRingList, addRing, expandRingList)

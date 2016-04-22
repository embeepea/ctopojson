#include <stdio.h>
#include <stdlib.h>
#include "arclist.h"

LIST_IMPLEMENTATION(ArcList, Arc *, newArcList, addArc, expandArcList)

int arcLength(int i, int j, int size) {
  if (j <= i) { 
    j += size;
  }
  return j - i + 1;
}


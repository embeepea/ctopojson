#include <stdlib.h>
#include <stdio.h>
#include "intlist.h"
#include "list.h"

LIST_IMPLEMENTATION(IntList, int, newIntList, addInt, expandIntList)

void printIntList(IntList *intList) {
  int i;
  printf("[");
  for (i=0; i<intList->count; ++i) {
    if (i>0) { printf(","); }
    printf("%1d", intList->elements[i]);
  }
  printf("]");
}

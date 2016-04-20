#include <stdlib.h>
#include <stdio.h>
#include "intlist.h"

IntList *newIntList(int N) {
  IntList *intList = (IntList*)malloc(sizeof(IntList));
  intList->size = N;
  intList->count = 0;
  intList->v = (int*)malloc(sizeof(int)*N);
  return intList;
}

void expandIntList(IntList *intList) {
  int i, oldsize = intList->size;
  intList->size = 2*intList->size;
  int *newV = (int*)malloc(sizeof(int)*intList->size);
  for (i=0; i<intList->count; ++i) {
    newV[i] = intList->v[i];
  }
  free(intList->v);
  intList->v = newV;
}

int addInt(IntList *intList, int v) {
  int i = intList->count;
  ++(intList->count);
  if (intList->count >= intList->size) {
    expandIntList(intList);
  }
  intList->v[i] = v;
  return i;
}

void printIntList(IntList *intList) {
  int i;
  printf("[");
  for (i=0; i<intList->count; ++i) {
    if (i>0) { printf(","); }
    printf("%1d", intList->v[i]);
  }
  printf("]");
}

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

void outputIntList(FILE *fp, IntList *list) {
  int i;
  fprintf(fp,"[");
  for (i=0; i<list->count; ++i) {
    if (i>0) { fprintf(fp,","); }
    fprintf(fp, "%1d", list->elements[i]);
  }
  fprintf(fp,"]");
}

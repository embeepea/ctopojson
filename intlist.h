#ifndef INTLIST_H_INCLUDED
#define INTLIST_H_INCLUDED

typedef struct IntList {
  int *v;
  int size; // allocated size of v array
  int count;   // number of current populated elements in v array
} IntList;

IntList *newIntList(int N);
void expandIntList(IntList *intList);
int addInt(IntList *intList, int v);
void printIntList(IntList *intList);

#endif // INTLIST_H_INCLUDED

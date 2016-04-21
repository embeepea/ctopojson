#ifndef INTLIST_H_INCLUDED
#define INTLIST_H_INCLUDED

#include "list.h"

LIST_DECLARATIONS(IntList, int, newIntList, addInt, expandIntList)

void printListType(IntList *intList);

#endif // INTLIST_H_INCLUDED

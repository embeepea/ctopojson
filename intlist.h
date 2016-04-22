#ifndef INTLIST_H_INCLUDED
#define INTLIST_H_INCLUDED

#include <stdio.h>
#include "list.h"

LIST_DECLARATIONS(IntList, int, newIntList, addInt, expandIntList)

void printListType(IntList *intList);
void outputIntList(FILE *fp, IntList *list);

#endif // INTLIST_H_INCLUDED

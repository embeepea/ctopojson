#include <stdio.h>
#include "point.h"

int pointsEqual(Point *p1, Point *p2) {
  return (p1->x == p2->x) && (p1->y == p2->y);
}

void printPoint(Point *p) {
  printf("(%1d,%1d)", p->x, p->y);
}

#include <stdio.h>
#include "point.h"

int pointsEqual(Point *p1, Point *p2) {
  return (p1->x == p2->x) && (p1->y == p2->y);
}

int pointsCompareLess(Point *p1, Point *p2) {
  if (p1->x < p2->x) { return 1; }
  if (p1->x > p2->x) { return 0; }
  if (p1->y < p2->y) { return 1; }
  return 0;
}

void printPoint(Point *p) {
  printf("(%1d,%1d)", p->x, p->y);
}

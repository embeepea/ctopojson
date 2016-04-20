#include "point.h"

int pointsEqual(Point *p1, Point *p2) {
  return (p1->x == p2->x) && (p1->y == p2->y);
}

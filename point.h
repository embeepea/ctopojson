#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

typedef struct Point {
  int x, y;
} Point;

int pointsEqual(Point *p1, Point *p2);
void printPoint(Point *p);

#endif // POINT_H_INCLUDED

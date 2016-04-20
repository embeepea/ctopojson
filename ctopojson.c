#include "ogr_api.h"

#include "intlist.h"
#include "point.h"
#include "ring.h"
#include "ringlist.h"
#include "polygon.h"
#include "polygonlist.h"
#include "multipolygon.h"
#include "multipolygonlist.h"

char *typeToString(OGRwkbGeometryType type) {
  if (type == wkbUnknown) { return "wkbUnknown"; }
  if (type == wkbPoint) { return "wkbPoint"; }
  if (type == wkbLineString) { return "wkbLineString"; }
  if (type == wkbPolygon) { return "wkbPolygon"; }
  if (type == wkbMultiPoint) { return "wkbMultiPoint"; }
  if (type == wkbMultiLineString) { return "wkbMultiLineString"; }
  if (type == wkbMultiPolygon) { return "wkbMultiPolygon"; }
  if (type == wkbGeometryCollection) { return "wkbGeometryCollection"; }
  if (type == wkbNone) { return "wkbNone"; }
  if (type == wkbLinearRing) { return "wkbLinearRing"; }
  if (type == wkbPoint25D) { return "wkbPoint25D"; }
  if (type == wkbLineString25D) { return "wkbLineString25D"; }
  if (type == wkbPolygon25D) { return "wkbPolygon25D"; }
  if (type == wkbMultiPoint25D) { return "wkbMultiPoint25D"; }
  if (type == wkbMultiLineString25D) { return "wkbMultiLineString25D"; }
  if (type == wkbMultiPolygon25D) { return "wkbMultiPolygon25D"; }
  if (type == wkbGeometryCollection25D) { return "wkbGeometryCollection25D"; }
  return "???";
}

typedef struct State {
  double x[100000];
  double y[100000];
  double z[100000];

  RingList *ringList;
  PolygonList *polygonList;
  MultiPolygonList *multiPolygonList;
} State;

State *newState() {
  State *state = (State*)malloc(sizeof(State));
  state->ringList = newRingList(1024);
  state->polygonList = newPolygonList(1024);
  state->multiPolygonList = newMultiPolygonList(32);
  return state;
}

int double_to_int(double x) {
  return ((int)(x * 1e5));
}

void doubleXYToPoint(Point *p, double x, double y) {
  p->x = double_to_int(x);
  p->y = double_to_int(y);
}

Ring *doublePointArrayToRing(int n, double *x, double *y) {
  int i, ix, iy;
  int m = 0;
  Ring *ring = (Ring*)malloc(sizeof(Ring));
  ring->n = n;
  ring->points = (Point*)malloc(sizeof(Point)*n);
  for (i=0; i<n; ++i) {
    doubleXYToPoint(&(ring->points[m]), x[i], y[i]);
    if ((m == 0) || !pointsEqual(&(ring->points[m]), &(ring->points[m-1]))) {
      // eliminate consecutive identical points
      ++m;
    }
  }
  // if we got fewer than n distinct points, copy to new points array of correct length
  if (m < n) {
    Point *newPoints = (Point*)malloc(sizeof(Point)*m);
    for (i=0; i<m; ++i) {
      newPoints[i].x = ring->points[i].x;
      newPoints[i].y = ring->points[i].y;
    }
    free(ring->points);
    ring->points = newPoints;
    ring->n = m;
  }
}

int extractRingFromLineString(State *state, OGRGeometryH hGeometry) {
  int npoints = OGR_G_GetPoints(hGeometry,
                                state->x, sizeof(double),
                                state->y, sizeof(double),
                                state->z, sizeof(double));

  Ring *ring = doublePointArrayToRing(npoints, state->x, state->y);
  return addRing(state->ringList, ring);
}

void extractRingsFromPolygon(State *state, PolygonList *polygonList, OGRGeometryH hGeometry) {
  int ngeoms = OGR_G_GetGeometryCount(hGeometry);
  OGRwkbGeometryType gtype;
  int i;
  Polygon *polygon = newPolygon();
  for (i=0; i<ngeoms; ++i) {
    addInt(polygon->ringIndices,
           extractRingFromLineString(state, OGR_G_GetGeometryRef(hGeometry, i)));
  }
  addPolygon(polygonList, polygon);
}

void extractRingsFromMultiPolygon(State *state, OGRGeometryH hGeometry) {
  int ngeoms = OGR_G_GetGeometryCount(hGeometry);
  OGRGeometryH hSubGeometry;
  OGRwkbGeometryType gtype;
  int i;
  MultiPolygon *multiPolygon = newMultiPolygon();
  addMultiPolygon(state->multiPolygonList, multiPolygon);
  for (i=0; i<ngeoms; ++i) {
    hSubGeometry = OGR_G_GetGeometryRef(hGeometry, i);
    if (OGR_G_GetGeometryType(hSubGeometry) == wkbPolygon) {
      extractRingsFromPolygon(state, multiPolygon->polygonList, hSubGeometry);
    } else {
      printf("MultiPolygon contains a non-Polygon\n");
    }
  }
}

void extractRingsFromGeom(State *state, OGRGeometryH hGeometry) {
  OGRwkbGeometryType gtype = OGR_G_GetGeometryType(hGeometry);

  if (hGeometry == NULL) { return; }

  if (gtype == wkbPolygon) {
    extractRingsFromPolygon(state, state->polygonList, hGeometry);
  } else if (gtype == wkbMultiPolygon) {
    extractRingsFromMultiPolygon(state, hGeometry);
  } else {
    printf("strange geom type: %s\n", typeToString(gtype));
  }
}

void extractRingsFromLayer(State *state, OGRLayerH hLayer) {
  OGRFeatureH hFeature;
  OGRGeometryH hGeometry;
  OGR_L_ResetReading(hLayer);
  while( (hFeature = OGR_L_GetNextFeature(hLayer)) != NULL ) {
    hGeometry = OGR_F_GetGeometryRef(hFeature);
    if (hGeometry == NULL) { continue; }
    extractRingsFromGeom(state, hGeometry);
  }
}

int main(int argc, char **argv) {
  int N = 1;
  
  OGRDataSourceH hDS;
  OGRRegisterAll();

  char *layer = argv[1];

  char shpfile[128];

  sprintf(shpfile, "../%s.shp", layer);

  hDS = OGROpen( shpfile, FALSE, NULL );
  if( hDS == NULL ){
    printf( "Open failed.\n" );
    exit( 1 );
  }

  OGRLayerH hLayer;

  hLayer = OGR_DS_GetLayerByName( hDS, layer );

  State *state = newState();
  extractRingsFromLayer(state, hLayer);
  printf("In the end, got %1d rings\n", state->ringList->count);
  printf("            and %1d polygons\n", state->polygonList->count);

  int i, j;

  printf("Polygons:\n");
  for (i=0; i<state->polygonList->count; ++i) {
    printf("    %6d: ", i);
    printIntList(state->polygonList->polygons[i]->ringIndices);
    printf("\n");
  }
  printf("MultiPolygons:\n");
  for (j=0; j<state->multiPolygonList->count; ++j) {
    printf("    %6d:\n", j);
    for (i=0; i<state->multiPolygonList->multiPolygons[j]->polygonList->count; ++i) {
      printf("          %6d: ", i);
      printIntList(state->multiPolygonList->multiPolygons[j]->polygonList->polygons[i]->ringIndices);
      printf("\n");
    }
  }

  return 0;
}

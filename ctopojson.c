#include "ogr_api.h"

#include "geomtypes.h"
#include "intlist.h"
#include "point.h"
#include "ring.h"
#include "ringlist.h"
#include "polygon.h"
#include "polygonlist.h"
#include "multipolygon.h"
#include "multipolygonlist.h"
#include "pointhash.h"
#include "intpair.h"
#include "arc.h"
#include "arclist.h"

// set this to a positive number to process only the first MAXFEATURES features from input:
#define MAXFEATURES -1

// max number of distinct points; should be a prime number for optimal hashing experience
#define MAXPOINTS 5000011  //100003,500003,2000003,5000011,10000019

typedef struct State {
  double x[100000];
  double y[100000];
  double z[100000];

  RingList *ringList;
  PolygonList *polygonList;
  MultiPolygonList *multiPolygonList;
  PointHash *pointHash;
  IntPair **pointNeighbors;
} State;

State *newState() {
  int i;
  State *state = (State*)malloc(sizeof(State));
  state->ringList = newRingList(1024);
  state->polygonList = newPolygonList(1024);
  state->multiPolygonList = newMultiPolygonList(32);
  state->pointHash = newPointHash(MAXPOINTS);
  state->pointNeighbors = (IntPair**)malloc(sizeof(IntPair*)*MAXPOINTS);
  for (i=0; i<MAXPOINTS; ++i) {
    state->pointNeighbors[i] = NULL;
  }
  return state;
}

int double_to_int(double x) {
  return ((int)(x * 1e5));
}

void doubleXYToPoint(Point *p, double x, double y) {
  p->x = double_to_int(x);
  p->y = double_to_int(y);
}

int extractRingFromLineString(State *state, OGRGeometryH hGeometry) {
  int npoints = OGR_G_GetPoints(hGeometry,
                                state->x, sizeof(double),
                                state->y, sizeof(double),
                                state->z, sizeof(double));

  Ring *ring = doublePointArrayToRing(npoints, state->x, state->y);
  int i;
  for (i=0; i<ring->n; ++i) {
    getPointIndex(state->pointHash, &(ring->points[i]));
  }
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
  int n = 0;
  while( (hFeature = OGR_L_GetNextFeature(hLayer)) != NULL ) {
    ++n;
    if (MAXFEATURES > 0 && n >= MAXFEATURES) { break; }
    hGeometry = OGR_F_GetGeometryRef(hFeature);
    if (hGeometry == NULL) { continue; }
    extractRingsFromGeom(state, hGeometry);
  }
}

void joinRings(State *state) {
  int i;
  for (i=0; i<state->ringList->count; ++i) {
    traverseForJunctions(state->ringList->rings[i], state->pointHash, state->pointNeighbors);
  }
}

void cutRings(State *state) {
  int i;
  for (i=0; i<state->ringList->count; ++i) {
    setJunctions(state->ringList->rings[i], state->pointHash, state->pointNeighbors);
  }
}

int main(int argc, char **argv) {
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
  // at this point:
  //   state->rings contains all the rings from the layer
  //   state->polygonList contains all the Polygons (each polygon being a list of indices into the ring list;
  //     for each polygon the first ring is the exterior ring, additional rings are holes)
  //   state->multiPolygonList contains all the MultiPolygons; each MultiPolygon is essentially a thin
  //     wrapper around a PolygonList just like the above.

  joinRings(state);
  cutRings(state);

  {
    int M = state->ringList->count;
    int i,j;
    ArcList *arcList;

    printf("\n----------------------------------------------------------------------\n");
    for (i=0; i<M; ++i) {
      dumpRing(state->ringList->rings[i]);
      arcList = ringToArcList(state->ringList->rings[i]);
      for (j=0; j<arcList->count; ++j) {
        dumpArc(arcList->arcs[j]);
      }
      printf("\n----------------------------------------------------------------------\n");
    }

  }





  /*
  printf("In the end, got %1d rings\n", state->ringList->count);
  printf("            and %1d polygons\n", state->polygonList->count);

  int npoints=0, i;
  for (i=0; i<state->ringList->count; ++i) {
    npoints += state->ringList->rings[i]->n;
  }
  printf("            and %1d points\n", npoints);
  printf("                %1d unique points\n", state->pointHash->count);

  int nj = 0;
  for (i=0; i<MAXPOINTS; ++i) {
    if (state->pointNeighbors[i] == JUNCTION) { ++nj; }
  }
  printf("                %1d junctions\n", nj);
  */

  /*
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
  */

  return 0;
}

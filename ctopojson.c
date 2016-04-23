#include <stdio.h>
#include <stdlib.h>
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
#include "archash.h"
#include "ringarcs.h"
#include "layer.h"
#include "layerlist.h"

#define STREQ(a,b) (strcmp(a,b)==0)

#define QUANTIZATION 1.0e6

// set this to a positive number to process only the first MAXFEATURES features from input:
#define MAXFEATURES -1

// max number of distinct points; should be a prime number for optimal hashing experience
#define MAXPOINTS 5000011  //100003,500003,2000003,5000011,10000019

// max number of arcs; should also be a prime number
#define MAXARCS MAXPOINTS

typedef struct State {
  double x[100000];
  double y[100000];
  double z[100000];

  RingList *ringList;
  LayerList *layerList;
  PointHash *pointHash;
  IntPair **pointNeighbors;
  ArcHash *arcHash;  // hash which maps Arcs to indices into the arcIndices array below
  int *arcIndices;   // gives the index of an Arc in arcList below
  ArcList *arcList;  // the desired list of arcs being generated
} State;

State *newState() {
  int i;
  State *state = (State*)malloc(sizeof(State));
  state->ringList = newRingList(1024);
  state->pointHash = newPointHash(MAXPOINTS);
  state->arcHash = newArcHash(MAXARCS);
  state->layerList = newLayerList(1);
  state->pointNeighbors = (IntPair**)malloc(sizeof(IntPair*)*MAXPOINTS);
  for (i=0; i<MAXPOINTS; ++i) { state->pointNeighbors[i] = NULL; }
  state->arcIndices = (int*)malloc(sizeof(int)*MAXARCS);
  for (i=0; i<MAXARCS; ++i) { state->arcIndices[i] = -1; }
  state->arcList = newArcList(1024);
  return state;
}

int double_to_int(double x) {
  return ((int)(x * QUANTIZATION));
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

  Ring *ring = doublePointArrayToRing(npoints-1, state->x, state->y);
  int i;
  for (i=0; i<ring->n; ++i) {
    getPointIndex(state->pointHash, &(ring->points[i]));
  }
  return addRing(state->ringList, ring);
}

void extractRingsFromPolygon(State *state, Polygon *polygon, OGRGeometryH hGeometry) {
  int ngeoms = OGR_G_GetGeometryCount(hGeometry);
  OGRwkbGeometryType gtype;
  int i;
  for (i=0; i<ngeoms; ++i) {
    addInt(polygon->ringIndices,
           extractRingFromLineString(state, OGR_G_GetGeometryRef(hGeometry, i)));
  }
}

void extractRingsFromMultiPolygon(State *state, MultiPolygon *multiPolygon, OGRGeometryH hGeometry) {
  int ngeoms = OGR_G_GetGeometryCount(hGeometry);
  OGRGeometryH hSubGeometry;
  OGRwkbGeometryType gtype;
  int i;
  for (i=0; i<ngeoms; ++i) {
    hSubGeometry = OGR_G_GetGeometryRef(hGeometry, i);
    if (OGR_G_GetGeometryType(hSubGeometry) == wkbPolygon) {
      Polygon *polygon = newPolygon();
      extractRingsFromPolygon(state, polygon, hSubGeometry);
      addPolygon(multiPolygon->polygonList, polygon);
    } else {
      printf("MultiPolygon contains a non-Polygon\n");
    }
  }
}

void extractRingsFromFeature(State *state, Layer *layer, OGRFeatureH hFeature) {
  OGRGeometryH hGeometry;
  hGeometry = OGR_F_GetGeometryRef(hFeature);
  if (hGeometry == NULL) { return; }
  OGRwkbGeometryType gtype = OGR_G_GetGeometryType(hGeometry);
  Geom *geom;
  if (gtype == wkbPolygon) {
    geom = newGeom(GEOM_POLYGON);
    strcpy(geom->id, OGR_F_GetFieldAsString(hFeature, 1));
    extractRingsFromPolygon(state, geom->polygon, hGeometry);
    addGeom(layer->geomList, geom);
  } else if (gtype == wkbMultiPolygon) {
    geom = newGeom(GEOM_MULTIPOLYGON);
    strcpy(geom->id, OGR_F_GetFieldAsString(hFeature, 1));
    extractRingsFromMultiPolygon(state, geom->multiPolygon, hGeometry);
    addGeom(layer->geomList, geom);
  } else {
    printf("strange geom type: %s\n", typeToString(gtype));
  }
}

void extractRingsFromLayer(State *state, OGRLayerH hLayer) {
  OGRFeatureH hFeature;
  Layer *layer = newLayer();
  addLayer(state->layerList, layer);
  OGR_L_ResetReading(hLayer);
  int n = 0;
  while( (hFeature = OGR_L_GetNextFeature(hLayer)) != NULL ) {
    if (MAXFEATURES > 0 && n >= MAXFEATURES) { break; }
    extractRingsFromFeature(state, layer, hFeature);
    ++n;
  }
}

void joinRings(State *state) {
  int i;
  for (i=0; i<state->ringList->count; ++i) {
    traverseForJunctions(state->ringList->elements[i], state->pointHash, state->pointNeighbors);
  }
}

void cutRings(State *state) {
  int i;
  for (i=0; i<state->ringList->count; ++i) {
    setJunctions(state->ringList->elements[i], state->pointHash, state->pointNeighbors);
  }
}

int ones_complement(int x) {
  return -x - 1;
}

void dedupArcs(State *state) {
  int i, j, k, ai, rai;
  Ring *ring;
  Arc *arc;
  for (i=0; i<state->ringList->count; ++i) {
    ring = state->ringList->elements[i];
    ring->arcList = ringToArcList(ring);
    for (j=0; j<ring->arcList->count; ++j) {
      // * have we seen this arc before?
      //   * if yes, was it in the same direction?
      //     * if yes, let I = saved index for it
      //     * if no, let I = ones complement of index for it
      //   * if no, save it in the next slot, and let I be the index of that slot
      // * append I to this ring's list of arcs
      arc = ring->arcList->elements[j];
      ai = getArcIndex(state->arcHash, arc);
      if (state->arcIndices[ai] >= 0) {
        if (arcsEqual(state->arcList->elements[state->arcIndices[ai]], arc) > 0) {
          addInt(ring->arcIndices, state->arcIndices[ai]);
        } else {
          addInt(ring->arcIndices, ones_complement(state->arcIndices[ai]));
        }
      } else {
        rai = getReverseArcIndex(state->arcHash, arc);
        if (state->arcIndices[rai] >= 0) {
          if (arcsEqual(state->arcList->elements[state->arcIndices[rai]], arc) > 0) {
            addInt(ring->arcIndices, state->arcIndices[rai]);
          } else {
            addInt(ring->arcIndices, ones_complement(state->arcIndices[rai]));
          }
        } else {
          // we've not seen this arc before, so add it to our master list
          k = addArc(state->arcList, arc);
          state->arcIndices[ai] = k;
          addInt(ring->arcIndices, k);
        }
      }
    }
  }
}

void outputLayer(FILE *fp, State *state, Layer *layer) {
  int i, j, k;
  const char *id;
  Geom *geom;
  fprintf(fp,"{\"geometries\":[");
  for (i=0; i<layer->geomList->count; ++i) {
    Geom *geom = layer->geomList->elements[i];
    fprintf(fp,"{\"id\":\"%s\",", geom->id);
    if (geom->type == GEOM_POLYGON) {
      fprintf(fp,"\"type\":\"Polygon\",", id);
      fprintf(fp,"\"arcs\":[");
      for (j=0; j<geom->polygon->ringIndices->count; ++j) {
        if (j>0) { fprintf(fp,","); }
        outputIntList(fp, state->ringList->elements[geom->polygon->ringIndices->elements[j]]->arcIndices);
      }
      fprintf(fp,"]");
    } else if (geom->type == GEOM_MULTIPOLYGON) {
      fprintf(fp,"\"type\":\"MultiPolygon\",", id);
      fprintf(fp,"\"arcs\":[");
      for (k=0; k<geom->multiPolygon->polygonList->count; ++k) {
        fprintf(fp,"[");
        Polygon *polygon = geom->multiPolygon->polygonList->elements[k];
        for (j=0; j<polygon->ringIndices->count; ++j) {
          if (j>0) { fprintf(fp,","); }
          outputIntList(fp, state->ringList->elements[polygon->ringIndices->elements[j]]->arcIndices);
        }
        fprintf(fp,"]%s", k==geom->multiPolygon->polygonList->count-1 ? "" : ",");
      }
      printf("]");
    } else {
    }
    fprintf(fp,"}%s", i==layer->geomList->count-1 ? "" : ",");
  }
  fprintf(fp,"]}");
}

void output_topology_start(FILE *fp) {
  fprintf(fp, "{\"type\":\"Topology\",");
}
void output_transform(FILE *fp) {
  fprintf(fp, "\"transform\":{\"scale\":[%f,%f],\"translate\":[0,0]},",
          1/QUANTIZATION, 1/QUANTIZATION);
}
void output_objects_start(FILE *fp) {
  fprintf(fp, "\"objects\":{");
}
void output_topology_end(FILE *fp) {
  fprintf(fp, "}");
}
void output_objects_end(FILE *fp) {
  fprintf(fp, "},");
}
void output_arcs(FILE *fp, State *state) {
  int i;
  fprintf(fp, "\"arcs\":[");
  for (i=0; i<state->arcList->count; ++i) {
    if (i>0) { fprintf(fp,","); }
    outputEncodedArc(fp, state->arcList->elements[i]);
  }
  fprintf(fp, "]");
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

  ////////////////////////////////////////////////////


  // OGRFeatureDefnH hFDefn = OGR_L_GetLayerDefn(hLayer);
  // int iField;
  // printf("%1d fields\n", OGR_FD_GetFieldCount(hFDefn));
  // for( iField = 0; iField < OGR_FD_GetFieldCount(hFDefn); iField++ ) {
  //     OGRFieldDefnH hFieldDefn = OGR_FD_GetFieldDefn( hFDefn, iField );
  //     printf(OGR_Fld_GetNameRef(hFDefn));
  //     printf("\n");
  //     if( OGR_Fld_GetType(hFieldDefn) == OFTInteger )
  //       printf( "%s: Integer", OGR_Fld_GetNameRef(hFDefn));
  //     else if( OGR_Fld_GetType(hFieldDefn) == OFTReal )
  //       printf( "%s: Double", OGR_Fld_GetNameRef(hFDefn));
  //     else if( OGR_Fld_GetType(hFieldDefn) == OFTString )
  //       printf( "%s: String", OGR_Fld_GetNameRef(hFDefn));
  //     else
  //       printf( "%s: OTHER", OGR_Fld_GetNameRef(hFDefn));
  //   }

  ////////////////////////////////////////////////////





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
  dedupArcs(state);

  if (0) {
    int M = state->ringList->count;
    int i,j;
    ArcList *arcList;
    printf("\n----------------------------------------------------------------------\n");
    for (i=0; i<M; ++i) {
      dumpRing(state->ringList->elements[i]);
      arcList = ringToArcList(state->ringList->elements[i]);
      for (j=0; j<arcList->count; ++j) {
        dumpArc(arcList->elements[j]);
      }
      printf("\n----------------------------------------------------------------------\n");
    }
  }

  if (0) {
    int i,j;
    GeomList *geomList = state->layerList->elements[0]->geomList;
    for (i=0; i<geomList->count; ++i) {
      Geom *geom = geomList->elements[i];
      if (STREQ(geom->id,"060102010201")) {
        Polygon *polygon = geom->polygon;
        IntList *ringIndices = polygon->ringIndices;
        printf("Polygon 060102010201:\n");
        printf("  has %1d ring:  #%1d\n", ringIndices->count, ringIndices->elements[0]);
        Ring *ring = state->ringList->elements[ringIndices->elements[0]];
        dumpRing(ring);
        for (j=0; j<ring->arcIndices->count; ++j) {
          dumpArc(state->arcList->elements[ring->arcIndices->elements[j]]);
        }
      }
    }
    return 0;
  }

  //printf("In the end, got %1d rings\n", state->ringList->count);
  //printf("            and %1d geoms\n", state->layerList->elements[0]->geomList->count);
  //int npoints=0, i;
  //for (i=0; i<state->ringList->count; ++i) {
  //  npoints += state->ringList->elements[i]->n;
  //}
  //printf("            and %1d points\n", npoints);
  //printf("                %1d unique points\n", state->pointHash->count);
  //printf("            and %1d arcss\n", state->arcList->count);

  if (1) {
    FILE *fp = stdout;
    output_topology_start(fp);
    output_transform(fp);
    output_objects_start(fp);

    fprintf(fp, "\"%s\":", "h12");
    Layer *layer = state->layerList->elements[0];
    outputLayer(stdout, state, layer);

    output_objects_end(fp);
    output_arcs(fp, state);
    output_topology_end(fp);
  }


//    int i,j,k;
//    const char *id;
//    OGR_L_ResetReading(hLayer);
//    OGRFeatureH hFeature;
//    for (i=0; i<layer->geomList->count; ++i) {
//      hFeature = OGR_L_GetNextFeature(hLayer);
//      if (hFeature == NULL) { continue; }
//      id = OGR_F_GetFieldAsString(hFeature, 0);
//      Geom *geom = layer->geomList->elements[i];
//      if (geom->type == GEOM_POLYGON) {
//        printf("Polygon(%s)[\n", id);
//        for (j=0; j<geom->polygon->ringIndices->count; ++j) {
//          printf("  ");
//          printRingArcs(state->ringList->elements[geom->polygon->ringIndices->elements[j]]);
//          printf("\n");
//        }
//        printf("]\n");
//      } else if (geom->type == GEOM_MULTIPOLYGON) {
//        printf("MultiPolygon(%s)[\n", id);
//        for (k=0; k<geom->multiPolygon->polygonList->count; ++k) {
//          Polygon *polygon = geom->multiPolygon->polygonList->elements[k];
//          printf("  Polygon[\n");
//          for (j=0; j<polygon->ringIndices->count; ++j) {
//            printf("    ");
//            printRingArcs(state->ringList->elements[polygon->ringIndices->elements[j]]);
//            printf("\n");
//          }
//          printf("  ]\n");
//        }
//        printf("]\n");
//      } else {
//      }
//      
//    }

  /*
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
    printIntList(state->polygonList->elements[i]->ringIndices);
    printf("\n");
  }
  printf("MultiPolygons:\n");
  for (j=0; j<state->multiPolygonList->count; ++j) {
    printf("    %6d:\n", j);
    for (i=0; i<state->multiPolygonList->elements[j]->polygonList->count; ++i) {
      printf("          %6d: ", i);
      printIntList(state->multiPolygonList->elements[j]->polygonList->elements[i]->ringIndices);
      printf("\n");
    }
  }
  */

  return 0;
}

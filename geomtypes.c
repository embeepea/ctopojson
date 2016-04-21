#include "geomtypes.h"

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

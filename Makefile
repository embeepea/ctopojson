default: ctopojson

OFILES = \
  ctopojson.o \
  geomtypes.o \
  intlist.o  \
  point.o  \
  ring.o  \
  ringlist.o \
  polygon.o \
  polygonlist.o \
  multipolygon.o \
  multipolygonlist.o \
  pointhash.o \
  intpair.o \
  arc.o \
  arclist.o \
  archash.o \
  ringarcs.o \
  mod.o

HFILES = \
  geomtypes.h \
  intlist.h  \
  point.h  \
  ring.h  \
  ringlist.h \
  polygon.h \
  polygonlist.h \
  multipolygon.h \
  multipolygonlist.h \
  simplehash.h \
  pointhash.h \
  archash.h \
  intpair.h \
  arc.h \
  arclist.h \
  ringarcs.h \
  mod.h \
  list.h

ctopojson: ${OFILES}
	cc -o ctopojson ${OFILES} -lgdal

ctopojson.o: ctopojson.c ${HFILES}
	cc -c ctopojson.c

intlist.o: intlist.c intlist.h
	cc -c intlist.c

ring.o: ring.c ring.h
	cc -c ring.c

ringlist.o: ringlist.c ringlist.h
	cc -c ringlist.c

point.o: point.c point.h
	cc -c point.c

polygon.o: polygon.c polygon.h
	cc -c polygon.c

polygonlist.o: polygonlist.c polygonlist.h
	cc -c polygonlist.c

multipolygon.o: multipolygon.c multipolygon.h
	cc -c multipolygon.c

multipolygonlist.o: multipolygonlist.c multipolygonlist.h
	cc -c multipolygonlist.c

pointhash.o: pointhash.c pointhash.h
	cc -c pointhash.c

archash.o: archash.c archash.h
	cc -c archash.c

intpair.o: intpair.c intpair.h
	cc -c intpair.c

geomtypes.o: geomtypes.c geomtypes.h
	cc -c geomtypes.c

arc.o: arc.c arc.h
	cc -c arc.c

arclist.o: arclist.c arclist.h
	cc -c arclist.c

ringarcs.o: ringarcs.c ringarcs.h
	cc -c ringarcs.c

mod.o: mod.c mod.h
	cc -c mod.c

test: _always ctopojson
	./ctopojson h12

dtest: _always ctopojson
	./ctopojson dnstrms

utest: _always ctopojson
	./ctopojson upstrms

clean: _always
	/bin/rm -f *.o ctopojson


.PHONY: _always

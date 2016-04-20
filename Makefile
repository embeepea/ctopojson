ctopojson: ctopojson.o intlist.o  point.o  ring.o  ringlist.o polygon.o polygonlist.o multipolygon.o multipolygonlist.o
	cc -o ctopojson ctopojson.o intlist.o  point.o  ring.o  ringlist.o polygon.o polygonlist.o multipolygon.o multipolygonlist.o -lgdal

ctopojson.o: ctopojson.c intlist.h  point.h  ring.h  ringlist.h polygon.h
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

test: _always ctopojson
	./ctopojson h12

.PHONY: _always

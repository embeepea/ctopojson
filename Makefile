default: ctopojson

SRCS = \
  ctopojson.c       geomtypes.c          intlist.c        point.c         \
  ring.c            ringlist.c           polygon.c        polygonlist.c   \
  multipolygon.c    multipolygonlist.c   pointhash.c      intpair.c       \
  arc.c             arclist.c            archash.c        ringarcs.c      \
  mod.c             layer.c              layerlist.c      geom.c          \
  geomlist.c

OBJS = $(SRCS:.c=.o)

include $(SRCS:.c=.d)

ctopojson: $(OBJS)
	cc -o ctopojson $(OBJS) -lgdal

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

%.d: %.c
	@set -e; rm -f $@; \
	 $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	 sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	 rm -f $@.$$$$

test: _always ctopojson
	@./ctopojson nhucs

square: _always ctopojson
	@./ctopojson square

huc: _always ctopojson
	@./ctopojson huc

dtest: _always ctopojson
	./ctopojson dnstrms

utest: _always ctopojson
	./ctopojson upstrms

clean: _always
	/bin/rm -f *.o *.d ctopojson

.PHONY: _always

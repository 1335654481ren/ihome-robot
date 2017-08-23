
CXX=g++

CFLAGS=-Iinclude 
#CFLAGS=-Iinclude  `pkg-config --cflags libpulse-simple`

#LIBS=-lespeak `pkg-config --libs libpulse-simple`

#SLIBS=lib/share/libespeak.so `pkg-config --libs libpulse-simple` 
SLIBS=lib/share/libespeak.so

OBJECTS=test.o

TARGETS=test

$(TARGETS):$(OBJECTS)
	$(CXX) $^ $(SLIBS) $(LIBS) -o $@

.cpp.o:
	$(CXX) -c $< $(CFLAGS) -o $@

.c.o:
	$(CC) -c $< $(CFLAGS) -o $@

clean:
	-rm -rf $(TARGETS) $(OBJECTS)

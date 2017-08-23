CXX=g++
CFLAGS=-Iinclude -Iinclude/sonic -Iinclude/speech_tools -Iinclude/soundtouch -Iinclude/festival -Iinclude/utf8 `pkg-config --cflags libpulse-simple`

LIBS=-lpthread -lvorbisenc -lm -logg -lmp3lame -lsndfile -lncurses `pkg-config --libs libpulse-simple`

SLIBS=lib/libekho.a lib/libSoundTouch.a lib/libFestival.a lib/libestools.a lib/libeststring.a lib/libestbase.a

OBJECTS=ekhotest.o

TARGETS=testekho

$(TARGETS):$(OBJECTS)
	$(CXX) $^ $(SLIBS) $(LIBS) -o $@
.cpp.o:
	$(CXX) -c $< $(CFLAGS) -o $@
.c.o:
	$(CC) -c $< $(CFLAGS) -o $@
clean:
	-rm -rf $(TARGETS) $(OBJECTS)

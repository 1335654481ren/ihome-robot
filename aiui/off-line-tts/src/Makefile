#common makefile header

DIR_INC = ../../include
DIR_BIN = ../../bin
DIR_LIB = ../../libs

TARGET	= tts_sample
BIN_TARGET = ./$(TARGET)

CROSS_COMPILE =
CFLAGS = -g -Wall -I$(DIR_INC)

LDFLAGS := -L$(DIR_LIB)/x64
LDFLAGS += -lmsc -lrt -ldl -lpthread -lasound

OBJECTS := $(patsubst %.c,%.o,$(wildcard *.c))

$(BIN_TARGET) : $(OBJECTS)
	$(CROSS_COMPILE)gcc $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o : %.c
	$(CROSS_COMPILE)gcc -c $(CFLAGS) $< -o $@
clean:
	@rm -f *.o $(BIN_TARGET) tts.wav

.PHONY:clean

#common makefile foot

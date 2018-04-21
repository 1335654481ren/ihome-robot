PROGS=http_server 

all:${PROGS}

SRC=${shell pwd}/src
OUTPUT=${shell pwd}/bin

MAKE_BIN_DIR := ${shell mkdir -p $(OUTPUT) }
MAKE_EXAMPLE_HTTP_BIN_DIR := $(OUTPUT)/

CXXFLAGS+=-g -std=c++11 
LDFLAGS+=-lboost_system -lboost_thread -lpthread

http_server:${SRC}/main.o\
    ${SRC}/http_server.o\
    ${SRC}/connection.o\
    ${SRC}/request_handler.o\
    ${SRC}/request_parser.o\
    ${SRC}/reply.o\
    ${SRC}/connection_manager.o\
    ${SRC}/mime_types.o

	
	@${CXX} ${CXXFLAGS}  -o ./$@   $^  ${LDFLAGS}

clean:
	@rm -rf ${OUTPUT} ${SRC}/*.o http_server 

.PHONY: all clean

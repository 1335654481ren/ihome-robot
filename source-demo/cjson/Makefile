# Readme:
# use: make dir
# use: make c
# use: make clean
# use: make clean-dist
# use: make m
# use: make help

# Targets of the build, ___needadd___
OUTPUT_FILE      = app

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG            = 0
USE_MEM_TEST     = 0
STRIP_RELEASE    = 0

# cross compile
CROSS_COMPILE    = 

CC               := $(CROSS_COMPILE)gcc
CXX              := $(CROSS_COMPILE)g++
LD               := $(CROSS_COMPILE)g++
AR               := $(CROSS_COMPILE)ar cr
RANLIB           := $(CROSS_COMPILE)ranlib
STRIP            := $(CROSS_COMPILE)strip

# export
export DEBUG
export USE_MEM_TEST
export CROSS_COMPILE

# mkdir and find files
MAKE             = make
MAKE_DIR         = $(PWD)
SRC_DIR          = $(MAKE_DIR)/src/
OBJ_DIR          = $(MAKE_DIR)/obj/
LIB_DIR          = $(MAKE_DIR)/lib/
INCLUDE_DIR      = $(MAKE_DIR)/include
RELEASE_DIR      = $(MAKE_DIR)/
OUTPUT_DIR       = $(RELEASE_DIR)

# auto search src_dir/subfile_dir
SRC_APP          = $(shell find $(SRC_DIR) -maxdepth 2 \( ! -regex '.*/\..*' \) -type d)

# include dir
INCLUDE          := -I$(INCLUDE_DIR) 
INCLUDE          += $(foreach dir, $(SRC_APP), -I$(dir))
INCLUDE          += -I/usr/include/g++-2 -I/usr/local/include -I/usr/include -I/usr/include/opencv
INCLUDE          += 

# lib dir
LIB              := -L$(LIB_DIR)
LIB              += -lpthread -lz
# add lib file, ___needadd___
SRC_DIR_ALL      += $(foreach dir, $(SRC_APP), $(dir))
vpath            %.cpp $(SRC_DIR_ALL)
vpath            %.c   $(SRC_DIR_ALL)
vpath            %.o $(OBJ_DIR)
vpath            %.d $(OBJ_DIR)

SRC_FILES        := $(foreach dir, $(SRC_DIR_ALL), $(wildcard $(dir)/*.cpp $(dir)/*.c))
SRC_FILES        := $(notdir $(SRC_FILES))
OBJ_FILES        := $(addsuffix .o,$(basename ${SRC_FILES}))
DEP_FILES        := $(subst .o,.d,${OBJ_FILES})

DEBUG_CFLAGS     := -Wall -g -O0 -DDEBUG
DEBUG_CXXFLAGS   := ${DEBUG_CFLAGS} 
DEBUG_LDFLAGS    := -g

RELEASE_CFLAGS   := -Wall -O3 -DNDEBUG
RELEASE_CXXFLAGS := ${RELEASE_CFLAGS}
RELEASE_LDFLAGS  :=

ifeq (${DEBUG}, 1)
CFLAGS           := ${DEBUG_CFLAGS}
CXXFLAGS         := ${DEBUG_CXXFLAGS}
LDFLAGS          := ${DEBUG_LDFLAGS}
else
CFLAGS           := ${RELEASE_CFLAGS}
CXXFLAGS         := ${RELEASE_CXXFLAGS}
LDFLAGS          := ${RELEASE_LDFLAGS}
endif

OUT              = $(OUTPUT_DIR)$(OUTPUT_FILE)

# Makefile code common to all platforms, ___needadd___
USR_DEFINE       = 
#USR_DEFINE       += -DTIXML_USE_STL

ifeq ($(USE_MEM_TEST), 1)
USR_DEFINE       += -DDEBUG_EN
USR_DEFINE       += -DMEM_TEST
USR_DEFINE       += -DPC_SIMULATE
endif

# last cxxflags
CXXFLAGS         := ${CXXFLAGS} ${USR_DEFINE}

.PHONY: config dir help c clean clean-dist

# make OUT
all: \
	dir \
	BEFOREBUILD \
	$(OUT)

$(OUT): $(OBJ_FILES)
	@$(CXX) -o $@ $(LDFLAGS) $(SLIBS) $(addprefix $(OBJ_DIR),$(notdir $^)) $(LIB) $(USE_ELF2FLT)
	@echo -e "$(CXX) $(LDFLAGS)-o $@ $(USE_ELF2FLT)\n"
ifeq (${STRIP_RELEASE}, 1)
	${STRIP} ${OUT}
endif
ifeq (${COMPRESSED}, 1)
	$(CROSS_COMPILE)flthdr -z ${OUT}
endif

%.o: %.cpp %.d
#	@echo -e "building: $(notdir $@) ...\n"
	@$(CXX) -c $(CXXFLAGS) $< $(INCLUDE) -o $(OBJ_DIR)$@
	@echo -e "$(CXX) -c $(CXXFLAGS) $(notdir $<) -o $(notdir $@)"

%.o: %.c %.d
	@echo -e "building: $(notdir $@) ...\n"
	@$(CXX) -c $(CXXFLAGS) $< $(INCLUDE) -o $(OBJ_DIR)$@
	@echo -e "$(CXX) -c $(CXXFLAGS) $(notdir $<) -o $(notdir $@)"

BEFOREBUILD: 
ifneq ($(MAKECMDGOALS), c)
ifneq ($(MAKECMDGOALS), m)
ifneq ($(MAKECMDGOALS), dir)
ifneq ($(MAKECMDGOALS), help)
ifneq ($(MAKECMDGOALS), clean)
ifneq ($(MAKECMDGOALS), clean-dist)
sinclude $(addprefix $(OBJ_DIR),$(DEP_FILES))
endif
endif
endif
endif
endif
endif

$(OBJ_DIR)%.d: %.cpp
#	@echo -e "building: $(notdir $@) ...\n"
	@$(CXX) $< $(INCLUDE) -MM -MD -o $@
#	@echo -e "$(CXX) $(notdir $<) -MM -MD -o $(notdir $@)"

$(OBJ_DIR)%.d: %.c
#	@echo -e "building: $(notdir $@) ...\n"
	@$(CXX) $< $(INCLUDE) -MM -MD -o $@
#	@echo -e "$(CXX) $(notdir $<) -MM -MD -o $(notdir $@)"

dir:
	@-mkdir -p $(SRC_DIR)
	@-mkdir -p $(OBJ_DIR)
	@-mkdir -p $(LIB_DIR)
	@-mkdir -p $(INCLUDE_DIR)
	@-mkdir -p $(RELEASE_DIR)
	@echo ' '

m: valgrind
valgrind:
#	valgrind --log-file=valReport --tool=memcheck --leak-check=yes --show-reachable=yes --leak-resolution=high --undef-value-errors=yes $(OUT)
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes --leak-resolution=high --undef-value-errors=yes $(OUT)

c:
	-rm -f core $(OBJ_DIR)* *.d *.o
	-rm -f core $(OUT) $(OUT).gdb
	@echo ' '

clean: 
	-rm -f core $(OBJ_DIR)* *.d *.o
	-rm -f core $(OUT) $(OUT).gdb
	@echo ' '

clean-dist:
	-rm -rf ${OBJ_DIR} ${RELEASE_DIR}
	@echo ' '

AWK = awk
SORT = sort
PR = pr
help:
	@ -$(MAKE) --print-data-base --question |		\
	 $(AWK) '/^[^.%][-A-Za-z0-9_]*:/{print substr($$1,1,length($$1)-1)}'|	\
	 $(SORT)|\
	 $(PR) --omit-pagination --width=80 --columns=4


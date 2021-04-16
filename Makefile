CC = g++

CC_FILES=image.cc ray.cc camera.cc sphere.cc scene.cc plan.cc parser.cc triangle.cc metaball.cc raybox.cc engine.cc
MAIN_FILE=main.cc
BIN=raytracer
OBJ_FILES = $(CC_FILES:.cc=.o)

CXX_FLAGS = -Wall -Wextra -std=c++2a -Werror=narrowing

# Release flags
RELEASE_CXX_FLAGS = -Werror -march=native -DNDEBUG -fopenmp
RELEASE_CXX_FLAGS += -Ofast # Standard compliance is no more
RELEASE_CXX_FLAGS += -march=native # Allowed to use CPU special instruction set
RELEASE_CXX_FLAGS += -flto # Link time optimizer
RELEASE_CXX_FLAGS += -funroll-loops # Self explanatory & enables other opti (rename register, fweb)
RELEASE_CXX_FLAGS += -fmerge-all-constants # Self explanatory but dangerous and not bug speed up
RELEASE_CXX_FLAGS += -fgcse-sm # subexpression elimination
RELEASE_CXX_FLAGS += -fgcse-las # Subexpression elimination
RELEASE_CXX_FLAGS += -fgcse-after-reload # Clean up redundant spilling
RELEASE_CXX_FLAGS += -fdelete-null-pointer-checks # Self expl
RELEASE_CXX_FLAGS += -fdevirtualize-speculatively # Opti call to virtual fonc
RELEASE_CXX_FLAGS += -fdevirtualize-at-ltrans # Opti call to virtual fonc
RELEASE_CXX_FLAGS += -fno-lifetime-dse # Dangerous if we use field of object after object destruction
RELEASE_CXX_FLAGS += -ftree-loop-im # Loop invariant motion on trees

# Debug flags
DEBUG_CXX_FLAGS = -g -O0

# Add flags for release or debug
CXX_FLAGS += $(RELEASE_CXX_FLAGS)
LDXX_FLAGS =

all: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) -o $(BIN) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)

%.o: %.cc
	$(CC) $(CPPFLAGS) $(CXX_FLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f $(OBJ_FILES)
	rm -f $(BIN)

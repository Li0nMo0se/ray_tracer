CC = g++

CC_FILES=image.cc ray.cc camera.cc sphere.cc scene.cc engine.cc plan.cc parser.cc triangle.cc metaball.cc
MAIN_FILE=main.cc
BIN=raytracer
OBJ_FILES = $(CC_FILES:.cc=.o)

CXX_FLAGS = -Wall -Wextra -std=c++2a -Werror=narrowing
RELEASE_CXX_FLAGS = -Werror -O3 -march=native -DNDEBUG
DEBUG_CXX_FLAGS = -g
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

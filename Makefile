CC = g++

CC_FILES=image.cc ray.cc camera.cc sphere.cc scene.cc engine.cc plan.cc parser.cc triangle.cc
MAIN_FILE=main.cc
BIN=raytracer
OBJ_FILES = $(CC_FILES:.cc=.o)

# CXX_FLAGS += -Wall -Wextra -Werror -O3 -march=native -std=c++2a
CXX_FLAGS += -Wall -Wextra -g -std=c++2a
LDXX_FLAGS =

all: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) -o $(BIN) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)

%.o: %.cc
	$(CC) $(CPPFLAGS) $(CXX_FLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f $(OBJ_FILES)
	rm -f $(BIN)

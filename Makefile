CC = g++

CC_FILES=image.cc vector.cc ray.cc camera.cc sphere.cc
MAIN_FILE=main.cc
BIN=raytracer
OBJ_FILES = $(CC_FILES:.cc=.o)

# CXX_FLAGS += -Wall -Wextra -Werror -O3 -g -std=c++17
CXX_FLAGS += -Wall -Wextra -O3 -g -std=c++17
LDXX_FLAGS =

all: $(OBJ_FILES)
	$(CC) $(MAIN_FILE) -o $(BIN) $(OBJ_FILES) $(CXX_FLAGS) $(LDXX_FLAGS)

%.o: %.cc
	$(CC) $(CPPFLAGS) $(CXX_FLAGS) -c $< -o $@

.PHONY: all clean

clean:
	rm -f $(OBJ_FILES)
	rm -f $(BIN)
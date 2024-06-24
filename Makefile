PROGRAM=snaze

COMPILER=g++

SRC_FILES=src/*.cpp
INCLUDE_FILES=-I src/ -I lib

CXX_FLAGS=-std=c++17 -Wall -pedantic

all: build

build: $(SRC_FILES)
	@if [ ! -d build ]; then \
		mkdir -p build; \
	fi

	$(COMPILER) $(CXX_FLAGS) $(INCLUDE_FILES) $(SRC_FILES) -o build/$(PROGRAM)	

clean:
	rm -rf build/



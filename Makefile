# STuring Makefile
# Konrad Talik <konrad.talik@slimak.matinf.uj.edu.pl>

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, bin/%.o, $(SOURCES)) main.o
INCLUDES = -I include
CXXFLAGS = -std=c++11 -Wall -pedantic -g $(INCLUDES)
PROGRAM = sturing

$(PROGRAM): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

bin/%.o: src/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(PROGRAM) $(OBJECTS)

# Tests params
TEXEC=./sturing
TPARAMS=-x [FILE_STEM].stu [INPUT_FILE]
TINPUT=examples
TIN=.tap

# Make Tests
include maketests/tests.mk


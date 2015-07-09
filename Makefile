# STuring Makefile

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, bin/%.o, $(SOURCES)) main.o
CC = g++
CXXFLAGS = -Wall -pedantic -g
PROGRAM = sturing

$(PROGRAM): $(OBJECTS)
	mkdir -p bin
	$(CC) $^ -o $@ -I include

%.o: %.cpp
	$(CC) -c $< -o $@ -I include

bin/%.o: src/%.cpp
	$(CC) -c $< -o $@ -I include

clean:
	rm $(PROGRAM) $(OBJECTS)

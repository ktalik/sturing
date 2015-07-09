# STuring Makefile

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%.cpp, bin/%.o, $(SOURCES)) main.o
CC = g++
CXXFLAGS = -Wall -pedantic -g
PROGRAM = sturing

$(PROGRAM): $(OBJECTS)
	$(CC) $^ -o $@ -I include

%.o: %.cpp
	$(CC) -c $< -o $@ -I include

bin/%.o: src/%.cpp
	mkdir -p bin
	$(CC) -c $< -o $@ -I include

clean:
	rm $(PROGRAM) $(OBJECTS)

# ./Makefile
# STuring makefile.

# Machine-related clases.
MAC=$(wildcard machine/*.cpp)

# Interpreter-related classes.
INT=$(wildcard interpreter/*.cpp)

# Utiility classes.
UTI=$(wildcard util/*.cpp)

OBJECTS=$(patsubst %.cpp, %.o, $(MAC) $(INT) $(UTI))
CC=g++
FLAGS=-Wall -pedantic
PROGRAM=sturing

# Link
$(PROGRAM): $(OBJECTS) sturing.cpp
	$(CC) $^ -o $@ -I ./

# Compile
%.o: %.cpp
	$(CC) -c $< -o $@ -I ./

%.o: %.cpp %.hpp
	$(CC) -c $< -o $@ -I ./

# Rebuild
rebuild: clean $(PROGRAM)

# Clean
clean:
	rm -rf $(PROGRAM) $(OBJECTS)

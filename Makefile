# ./Makefile
# STuring makefile.

# Machine-related clases.
MAC=$(wildcard machine/*.cpp)

# Interpreter-related classes.
INT=$(wildcard interpreter/*.cpp)

# Utiility classes.
UTI=$(wildcard util/*.cpp)

OBJECTS=$(patsubst %.cpp, %.o, $(MAC) $(INT) $(UTI)) main.o
CC=g++
FLAGS=-Wall -pedantic -g
PROGRAM=sturing

# Link
$(PROGRAM): $(OBJECTS)
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

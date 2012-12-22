# wrpg auto-makefile

MAC=$(wildcard mac/*.cpp)
INT=$(wildcard int/*.cpp)
OBJECTS=$(patsubst %.cpp, %.o, $(MAC) $(INT))
CC=g++
FLAGS=-Wall -pedantic
PROGRAM=sturing

# Link

$(PROGRAM): $(OBJECTS) sturing.cpp
	$(CC) $^ -o $@

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

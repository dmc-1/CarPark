# Source file
SRC=source/main.cpp

# Output
EXEC=build/app

# Build settings
CC=g++ -std=c++11

# SDL flags
SDLFLAGS=-I/usr/include/SDL2 -L/usr/lib/x86_64-linux-gnu -lSDL2 -lSDL2main 

all:
	$(CC) $(SRC) -o $(EXEC) $(SDLFLAGS)
run:
	$(EXEC)

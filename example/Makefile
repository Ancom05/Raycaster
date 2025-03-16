CC = g++
FLAGS = -O2
# FLAGS = -g -ggdb -std=c11
INCLUDE = -lm -lSDL 
S_DIR = src
# S_FILES=$(wildcard src/*.c)
B_DIR=build
EXEC=./$(B_DIR)/comocaster

all:Build #scelta predefinita se scrivi solo make

Build:
	$(CC) $(FLAGS) raycaster_flat.cpp quickcg.cpp $(INCLUDE) -o $(EXEC) 

run:Build #Chiama build e poi esegue con la variabile exec
	$(EXEC)

# clean:
#   rm -rf $(B_DIR)/*
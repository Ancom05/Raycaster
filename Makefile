CC = gcc
FLAGS = -O2 -std=c11
# FLAGS = -g -ggdb -std=c11
INCLUDE = -lm -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lcurl
S_DIR = src
# S_FILES=$(wildcard src/*.c)
B_DIR=build
EXEC=./$(B_DIR)/comocaster

all:Build #scelta predefinita se scrivi solo make

Build:
	$(CC) $(FLAGS) main_notextures.c $(INCLUDE) -o $(EXEC) 

run:Build #Chiama build e poi esegue con la variabile exec
	$(EXEC)

# clean:
#   rm -rf $(B_DIR)/*
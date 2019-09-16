OBJS = ./*/pio-texture.c ./*/pio-text-font.c ./*/pio-window.c ./*/miner.c ./*/camera.c ./*/tile.c ./*/level.c ./*/controls.c render.c main.c
CC = gcc
COMPILER_FLAGS = -w
LINKER_FLAGS = ./libs/libSDL2.so ./libs/libSDL2_image.so ./libs/libSDL2_mixer.so ./libs/libSDL2_ttf.so ./libs/libfreetype.so -L.

LD_LIBRARY_PATH = ./libs
export LD_LIBRARY_PATH

OBJ_NAME = app
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) | ./app

run : 
	./app




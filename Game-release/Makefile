#OBJS specifies which files to compile as part of the project
OBJS = ./*/pio-texture.c ./*/pio-text-font.c ./*/pio-window.c ./*/miner.c ./*/camera.c ./*/tile.c ./*/level.c ./*/controls.c render.c main.c

#CC specifies which compiler we're using
CC = gcc

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = libSDL2.so libSDL2_image.so libSDL2_mixer.so libSDL2_ttf.so libfreetype.so -L. `./sdl2-config --static-libs`

#-L./buildSDL/build/*.la -L./buildTTF/*.la -L./buildMixer/*.la -L./buildImg/*.la -lm

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = app

LD_LIBRARY_PATH = ./
export LD_LIBRARY_PATH
#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) | ./app




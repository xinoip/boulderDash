![Showcase](https://github.com/xinoip/boulderDash/blob/readme-assets/ast2.gif "Showcase")

# Boulderdash Clone 

Clone of famous commodore 64 game, boulderdash, made in C with SDL2 library.

## Level Editor

Level Editor that i made is [here](https://github.com/xinoip/boulderdash-level-editor).

### Controls

Arrow keys for movement.
R to reset level.
P to pause game.

## Compiling and Running Release Version

Install the release version and extract the files.

Make the makefile, you don't need anything else installed.

#### For pure dev version

Clone the master branch, make the makefile.

You will need libSDL2, libSDL2-mixer, libSDL2-image, libSDL2-ttf and freetypefont library.
In release version you do not need them.

## Library Path

If you run the game using makefile, there will be no problem.

But if you want to run the game without makefile set your LD_LIBRARY_PATH:

```bash
export LD_LIBRARY_PATH:$LD_LIBRARY_PATH:./libs
```

![Rip](https://github.com/xinoip/boulderDash/blob/readme-assets/ast1.gif "Rip")
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

typedef enum {false, true} bool;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const char *TITLE = "PioGame";

bool init();
bool loadMedia();
void closeAll();

//Loading individual images as surfaces
SDL_Surface * loadSurface(char *path);

//Main window
SDL_Window *gWindow = NULL;

//Main surface in window
SDL_Surface *gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface *gPNGSurface = NULL;

bool init() {

    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {  //Init SDL
        printf("SDL Could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;

    } else {    
        gWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_HEIGHT, SCREEN_WIDTH, SDL_WINDOW_SHOWN);
        if(gWindow == NULL) { //Create window
            printf("Window could not initialize! SDL Error: %s\n", SDL_GetError());
            success = false;

        } else {
            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags)) & imgFlags) { //Init SDL_image
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;

            } else { //Create surface on window
                gScreenSurface = SDL_GetWindowSurface(gWindow);

            }

        }

    }

    return success;

}

bool loadMedia() {

    bool success = true;

    gPNGSurface = loadSurface("./src-imgs/loaded.png");
    if(gPNGSurface == NULL) {
        printf("Failed to load PNG image!\n");
        success = false;

    }

    return success;

}

void closeAll() {

    SDL_FreeSurface(gPNGSurface);
    gPNGSurface = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();

}

SDL_Surface * loadSurface(char *path) {

    SDL_Surface *optimizedSurface = NULL;
    
    SDL_Surface *loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());

    } else {
        optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);
        if(optimizedSurface == NULL) {
            printf("Unable to optimize image %s! SDL Error: %s\n", path, IMG_GetError());

        }

        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;

    }

    return optimizedSurface;

}

int main(int argc, char *args[]) {
    if(!init()) { //Init phase
        printf("Failed to initialize!\n");

    } else {
        if(!loadMedia()) {
            printf("Failed to load media!\n");

        } else {
            bool quit = false;
            SDL_Event e;

            while(!quit) { //Game loop
                while(SDL_PollEvent(&e) != 0) { //Event queue loop
                    if(e.type == SDL_Quit) {
                        quit = true;

                    }

                }

                SDL_BlitSurface(gPNGSurface, NULL, gScreenSurface, NULL);

                SDL_UpdateWindowSurface(gWindow);

            }
            
        }

    }

    closeAll();

    return 0;

}
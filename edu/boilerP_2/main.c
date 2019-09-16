#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./pio-texture.h"
#include "./pio-text-font.h"

const char *TITLE = "pioGame";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef enum {false, true} bool;

bool init();
bool loadMedia();
void closeAll();

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
TTF_Font *gFont = NULL;

pioTexture_t playerTexture;
pioTextFont_t mainText;

bool init() {

    bool success = true;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;

    } else {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!\n");

        }

        gWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;

        } else {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if(gRenderer == NULL) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;

            } else {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init( imgFlags ) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;

				}

                if(TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;

                }

            }

        }

    }

    return success;

}

bool loadMedia() {

    bool success = true;

    gFont = TTF_OpenFont("../src-imgs/lazy.ttf", 28);
    if(gFont == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;

    }

    /* FIRST LOAD FONTS (ALWAYS!!!!) */

    SDL_Color fColor = {0, 0, 0};
    mainText = loadPioTextFont("abc", fColor, gFont, gRenderer);
    resizePioTexture(&(mainText.texture), 128, 128);
    if(mainText.texture.texture == NULL) {
        printf("Failed to load mainText text image!\n");
        success = false;

    }

    playerTexture = loadPioTexture("../src-imgs/playerTexture.png", gRenderer);
    resizePioTexture(&playerTexture, 128, 128);
    if(playerTexture.texture == NULL) {
        printf("Failed to load playerTexture image!\n");
        success = false;

    }

    return success;

}

void closeAll() {

    destroyPioTexture(&playerTexture);

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}

int main(int argc, char *args[]) {

    if(!init()) {
        printf("Failed to initialize!\n");

    } else {
        if(!loadMedia()) {
            printf("Failed to load media!\n");

        } else {
            bool quit = false;

            SDL_Event e;

            while(!quit) {
                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;

                    }

                }

                SDL_RenderClear(gRenderer);
                
                

                renderPioTexture(mainText.texture, 40, 40, gRenderer);
                renderPioTexture(playerTexture, 150, 150, gRenderer);

                SDL_RenderPresent(gRenderer);

                //quit = true;

            }

        }
        
    }

    closeAll();

    return 0;

}
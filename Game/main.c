#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "./base.h"
#include "./wrappers/pio-texture.h"
#include "./wrappers/pio-text-font.h"
#include "./wrappers/pio-window.h"
#include "./objects/tile.h"
#include "./objects/miner.h"
#include "./objects/camera.h"
#include "./objects/level.h"
#include "./render.h"
#include "./objects/controls.h"

#define INIT_WIDTH 640
#define INIT_HEIGHT 480

#define INIT_MINER_ROW 2
#define INIT_MINER_COL 3

miner_t miner;
camera_t camera;
pioWindow_t gameWindow;
level_t currLevel;

const char *TITLE = "pioGame";

SDL_Renderer *gRenderer = NULL;

bool init();
void closeAll();

bool init() {

    bool success = true;

    miner = createMiner(INIT_MINER_ROW, INIT_MINER_COL);

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;

    } else {
        if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!\n");

        }

        SDL_Window *gWindow = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, INIT_WIDTH, INIT_HEIGHT, SDL_WINDOW_RESIZABLE);
        if(gWindow == NULL) {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;

        } else {
            gameWindow.window = gWindow;
            updateWindowDims(&gameWindow);
            camera = createCamera(gameWindow);
            gRenderer = SDL_CreateRenderer(gameWindow.window, -1, SDL_RENDERER_ACCELERATED);
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

void closeAll() {

    closeMedia();

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    destroyPioWindow(&gameWindow);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}

int main(int argc, char *args[]) {

    Uint32 lastTime = 0, currentTime;
    Uint32 levelTime = 0, levelLastTime;
    if(!init()) {
        printf("Failed to initialize!\n");

    } else {
        if(!loadMedia(gRenderer)) {
            printf("Failed to load media!\n");

        } else {
            bool quit = false;

            SDL_Event e;

            fillLevel(&currLevel, "./assets/maps/mapB.txt");
            //fillLevel(&currLevel, "./assets/maps/albu.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);

            while(!quit) {
                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;

                    } else if(e.type == SDL_KEYDOWN) {
                        moveMiner(&miner, &currLevel, e);
                        switch (e.key.keysym.sym) {
                        case SDLK_a:
                            fillLevel(&currLevel, "./assets/maps/mapA.txt");
                            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
                            break;
                        case SDLK_b:
                            fillLevel(&currLevel, "./assets/maps/albu.txt");
                            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
                            break;
                        case SDLK_c:
                            fillLevel(&currLevel, "./assets/maps/mapX.txt");
                            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
                            updateGameBar(currLevel, gRenderer);
                            break;
                        default:
                            break;
                        }

                    } else if(e.type == SDL_WINDOWEVENT) {
                        switch (e.window.event) {
                        case SDL_WINDOWEVENT_RESIZED:
                            updateWindowDims(&gameWindow);
                            updateCamera(&camera, gameWindow);
                            break;
                        
                        default:
                            break;
                        }
                    }

                }
                
                currentTime = SDL_GetTicks();
                if(currentTime > lastTime + 200) {
                    updateMap(&currLevel);
                    lastTime = currentTime;    

                }

                levelTime = SDL_GetTicks();
                if(levelTime > levelLastTime + 1000) {
                    currLevel.timeLimit--;
                    levelLastTime = levelTime;
                    
                }

                updateGameBar(currLevel, gRenderer);

                updateCameraPosition(&camera, miner.row, miner.col);

                renderMap(currLevel, camera, gameWindow, gRenderer);

                SDL_RenderPresent(gRenderer);
                
            }

        }
        
    }

    closeAll();

    return 0;

}
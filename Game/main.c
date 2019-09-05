#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

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

#define INIT_WIDTH 1366
#define INIT_HEIGHT 720

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

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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
                //SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);

                //Initialize image loading
                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init( imgFlags ) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;

				}

                //Initialize font loading
                if(TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;

                }

                //Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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
    Mix_Quit();
    SDL_Quit();

}

void resetLevel(int levelNo) {
    startMusic();
    switch (levelNo) {
        case 1:
            fillLevel(&currLevel, "./assets/maps/cave_1.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);    
            break;
        case 2:
            fillLevel(&currLevel, "./assets/maps/cave_2.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);    
            break;
        case 3:
            fillLevel(&currLevel, "./assets/maps/cave_3.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            break;
        case 4:
            fillLevel(&currLevel, "./assets/maps/cave_4.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            break;
        case 5:
            fillLevel(&currLevel, "./assets/maps/cave_5.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            break;
        case 6:
            fillLevel(&currLevel, "./assets/maps/cave_6.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            break;
        case 7:
            fillLevel(&currLevel, "./assets/maps/cave_7.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            break;
        case 8:
            fillLevel(&currLevel, "./assets/maps/cave_8.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            break;
        case 9:
            fillLevel(&currLevel, "./assets/maps/cave_9.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            break;
        case 10:
            fillLevel(&currLevel, "./assets/maps/cave_10.txt");
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            break;              
        default:
            break;
    }
}

void gameOver() {
    updateGameBar(currLevel, gRenderer, 0);
    renderGameBar(currLevel, gameWindow, gRenderer, true);
    SDL_RenderPresent(gRenderer);
    SDL_Delay(10000);
    fillLevel(&currLevel, "./assets/maps/cave_1.txt");
    updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
    miner.level = 1;
    miner.lives = 10;
}

int main(int argc, char *args[]) {

    Uint32 lastTime = 0, currentTime = 0;
    Uint32 levelTime = 0, levelLastTime = 0;
    Uint32 waterTime = 0, waterLastTime = 0;
    if(!init()) {
        printf("Failed to initialize!\n");

    } else {
        if(!loadMedia(gRenderer)) {
            printf("Failed to load media!\n");

        } else {
            bool quit = false;
            bool pause = false;

            SDL_Event e;

            int playing10 = 0;
            int playing30 = 0;
            int playing60 = 0;

            fillLevel(&currLevel, "./assets/maps/cave_1.txt");
            miner.level = 1;
            updateMiner(&miner, currLevel.startMinerRow, currLevel.startMinerCol);
            startMusic();

            while(!quit) {
                if(playing60 == 0 && currLevel.timeLimit <= (currLevel.timeCpy * 10) / 100) {
                    printf("gone 60\n");
                    startMusic60();
                    playing60 = 1;
                } else if(playing30 == 0 && currLevel.timeLimit <= (currLevel.timeCpy * 20) / 100) {
                    printf("gone 30\n");
                    startMusic30();
                    playing30 = 1;
                } else if(playing10 == 0 && currLevel.timeLimit <= (currLevel.timeCpy * 30) / 100 && currLevel.timeLimit != 0) {
                    printf("gone 10\n");
                    startMusic10();
                    playing10 = 1;
                } 

                if(miner.lives <= 0) {
                    gameOver();
                }
                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;

                    } else if(e.type == SDL_KEYDOWN) {
                        if(!pause) {
                            moveMiner(&miner, &currLevel, e);
                        }
                        switch (e.key.keysym.sym) {
                        case SDLK_r:
                            resetLevel(miner.level);
                            break;
                        case SDLK_p:
                            if(pause == true) pause = false;
                            else if(pause == false) pause = true;
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
                
                // updates
                if(!pause) {
                    
                    currentTime = SDL_GetTicks();
                    if(currentTime > lastTime + 200) {
                        updateMap(&currLevel, &miner, camera, gameWindow, gRenderer);
                        lastTime = currentTime;    

                    }

                    levelTime = SDL_GetTicks();
                    if(levelTime > levelLastTime + 1000) {
                        currLevel.timeLimit--;
                        
                        levelLastTime = levelTime;
                        if(currLevel.timeLimit <= 0) {
                            currLevel.timeLimit = 0;
                            updateGameBar(currLevel, gRenderer, miner.lives);
                            renderGameBar(currLevel, gameWindow, gRenderer, pause);
                            printf("gone\n");
                            playing10 = 0;
                            playing30 = 0;
                            playing60 = 0;
                            stopMusic();
                            SDL_RenderPresent(gRenderer);
                            SDL_Delay(3000);
                            miner.lives--;
                            resetLevel(miner.level);

                        }

                    }
                    
                    waterTime = SDL_GetTicks();
                    if(waterTime > waterLastTime + currLevel.waterMs) {
                        updateWater(&currLevel);
                        
                        waterLastTime = waterTime;

                    }
                }
                
                updateGameBar(currLevel, gRenderer, miner.lives);

                updateCameraPosition(&camera, miner.row, miner.col, currLevel.row, currLevel.col);

                renderMap(currLevel, camera, gameWindow, gRenderer);

                renderGameBar(currLevel, gameWindow, gRenderer, pause);

                SDL_RenderPresent(gRenderer);
                
            }

        }
        
    }

    closeAll();

    return 0;

}
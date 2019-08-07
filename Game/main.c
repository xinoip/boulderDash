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

#define INIT_WIDTH 640
#define INIT_HEIGHT 480

#define INIT_MINER_ROW 2
#define INIT_MINER_COL 3

miner_t miner;
camera_t camera;
pioWindow_t gameWindow;
level_t currLevel;

const char *TITLE = "pioGame";

pioTexture_t playerTexture;
pioTexture_t emptyTexture;
pioTexture_t dirtTexture;
pioTexture_t borderTexture;
pioTexture_t rockTexture;

SDL_Renderer *gRenderer = NULL;
TTF_Font *gFont = NULL;

void updateMap() {

    for(int row = 0; row < currLevel.row; row++) {
        for(int col = 0; col < currLevel.col; col++) {
            if(currLevel.map[row][col] == 'R') {
                if(currLevel.map[row+1][col] == 'E') {
                    currLevel.map[row][col] = 'E';
                    currLevel.map[row+1][col] = 'B';
                    

                } else if(currLevel.map[row+1][col] == 'R') {
                    if(currLevel.map[row+1][col+1] == 'E') {
                        currLevel.map[row][col] = 'E';
                        currLevel.map[row+1][col+1] = 'B';

                    } else if(currLevel.map[row+1][col-1] == 'E') {
                        currLevel.map[row][col] = 'E';
                        currLevel.map[row+1][col-1] = 'B';

                    }

                }


            }

        }

    }

    for(int row = 0; row < currLevel.row; row++) {
        for(int col = 0; col < currLevel.col; col++) {
            if(currLevel.map[row][col] == 'B') {
                currLevel.map[row][col] = 'R';

            }

        }

    }

}

void renderMap() {
    SDL_RenderClear(gRenderer);
    //i specifies the Y coordinate of render pixel
    for(int i = 0; i < currLevel.row; i++) {
        //j specifies the X coordinate of render pixel
        for(int j = 0; j < currLevel.col; j++) {
            tile_t currentTile = createTile(i, j);
            tile_t cam = createTile(camera.row, camera.col);
            int diffX = (gameWindow.width / 2) - cam.center_x; 
            int diffY = (gameWindow.height / 2) - cam.center_y;
            
            if(isInsideCamera(gameWindow, camera, i, j)) {
                switch (currLevel.map[i][j]) {
                case 'W':
                    renderPioTexture(borderTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, gRenderer);
                    break;
                case 'P':
                    renderPioTexture(playerTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, gRenderer);
                    break;
                case 'D':
                    renderPioTexture(dirtTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, gRenderer);
                    break;
                case 'E':
                    renderPioTexture(emptyTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, gRenderer);
                    break;
                case 'R':
                    renderPioTexture(rockTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, gRenderer);
                    break;
                case 'B':
                    renderPioTexture(rockTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, gRenderer);
                    break;
                default:
                    break;
                }
            }

        }
    }

}
/* MUST REFACTOR THESE */

bool isValidCellToMove(const int x, const int y) {

    if(currLevel.map[x][y] == 'W') return false;

    return true;
}

bool init();
bool loadMedia();
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

bool loadMedia() {

    bool success = true;

    playerTexture = loadPioTexture("./assets/image/playerTexture.png", gRenderer);
    resizePioTexture(&playerTexture, TILE_WIDTH, TILE_HEIGHT);
    if(playerTexture.texture == NULL) {
        printf("Failed to load playerTexture image!\n");
        success = false;

    }

    emptyTexture = loadPioTexture("./assets/image/emptyTexture.png", gRenderer);
    resizePioTexture(&emptyTexture, TILE_WIDTH, TILE_HEIGHT);
    if(emptyTexture.texture == NULL) {
        printf("Failed to load emptyTexture image!\n");
        success = false;

    }

    dirtTexture = loadPioTexture("./assets/image/dirtTexture.png", gRenderer);
    resizePioTexture(&dirtTexture, TILE_WIDTH, TILE_HEIGHT);
    if(dirtTexture.texture == NULL) {
        printf("Failed to load dirtTexture image!\n");
        success = false;

    }

    borderTexture = loadPioTexture("./assets/image/borderTexture.png", gRenderer);
    resizePioTexture(&borderTexture, TILE_WIDTH, TILE_HEIGHT);
    if(borderTexture.texture == NULL) {
        printf("Failed to load borderTexture image!\n");
        success = false;

    }

    rockTexture = loadPioTexture("./assets/image/rockTexture.png", gRenderer);
    resizePioTexture(&rockTexture, TILE_WIDTH, TILE_HEIGHT);
    if(rockTexture.texture == NULL) {
        printf("Failed to load rockTexture image!\n");
        success = false;

    }

    return success;

}

void closeAll() {

    destroyPioTexture(&dirtTexture);
    destroyPioTexture(&borderTexture);
    destroyPioTexture(&playerTexture);
    destroyPioTexture(&emptyTexture);
    destroyPioTexture(&rockTexture);

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    destroyPioWindow(&gameWindow);

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

}

int main(int argc, char *args[]) {

    fillLevel(&currLevel, "./assets/maps/map2.txt");
    printLevel(currLevel);

    Uint32 lastTime = 0, currentTime;
    if(!init()) {
        printf("Failed to initialize!\n");

    } else {
        if(!loadMedia()) {
            printf("Failed to load media!\n");

        } else {
            bool quit = false;

            SDL_Event e;

            int counterMe = 1;

            while(!quit) {
                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;

                    } else if(e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            if(isValidCellToMove(miner.row-1, miner.col)) {
                                currLevel.map[miner.row][miner.col] = 'E';
                                currLevel.map[miner.row-1][miner.col] = 'P';
                                miner.row--;    
                            }
                            
                            break;
                        case SDLK_DOWN:
                            if(isValidCellToMove(miner.row+1, miner.col)) {
                                currLevel.map[miner.row][miner.col] = 'E';
                                currLevel.map[miner.row+1][miner.col] = 'P';
                                miner.row++;    
                            }
                           
                            break;
                        case SDLK_LEFT:
                            if(isValidCellToMove(miner.row, miner.col-1)) {
                                currLevel.map[miner.row][miner.col] = 'E';
                                currLevel.map[miner.row][miner.col-1] = 'P';
                                miner.col--;    
                            }
                            
                            break;
                        case SDLK_RIGHT:
                            if(isValidCellToMove(miner.row, miner.col+1)) {
                                currLevel.map[miner.row][miner.col] = 'E';
                                currLevel.map[miner.row][miner.col+1] = 'P';
                                miner.col++;    
                            }
                            
                            break;
                        case SDLK_a:
                            fillLevel(&currLevel, "./assets/maps/fooMap.txt");
                            miner.row = 1;
                            miner.col = 1;
                            break;
                        case SDLK_b:
                            fillLevel(&currLevel, "./assets/maps/map2.txt");
                            miner.row = 2;
                            miner.col = 3;
                            break;
                        case SDLK_c:
                            fillLevel(&currLevel, "./assets/maps/map3.txt");
                            miner.row = 1;
                            miner.col = 1;
                            break;
                        case SDLK_f:
                            updateMap();
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
                    updateMap();
                    lastTime = currentTime;    

                }

                //printf("%d\n", SDL_GetTicks());

                camera.col = miner.col;
                camera.row = miner.row;

                renderMap();

                SDL_RenderPresent(gRenderer);

            }

        }
        
    }

    closeAll();

    return 0;

}
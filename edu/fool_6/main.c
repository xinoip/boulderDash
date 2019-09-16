#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "./base.h"
#include "./pio-texture.h"
#include "./pio-text-font.h"
#include "./pio-window.h"
#include "./tile.h"
#include "./miner.h"
#include "./camera.h"


#define INIT_WIDTH 640
#define INIT_HEIGHT 480

#define INIT_MINER_ROW 2
#define INIT_MINER_COL 3

miner_t miner;
camera_t camera;
pioWindow_t gameWindow;

const char *TITLE = "pioGame";


pioTexture_t playerTexture;
pioTexture_t emptyTexture;
pioTexture_t dirtTexture;
pioTexture_t borderTexture;
pioTexture_t rockTexture;

SDL_Renderer *gRenderer = NULL;
TTF_Font *gFont = NULL;

/* INSTEAD USE game-map.h HEADER */
const LEVEL_ROW_SIZE = 13;
const LEVEL_COL_SIZE = 17;
char gameMap[13][17] = {
    {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
    {'W', 'D', 'D', 'D', 'R', 'R', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'R', 'E', 'P', 'R', 'R', 'R', 'R', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'E', 'E', 'E', 'R', 'E', 'E', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'R', 'E', 'E', 'E', 'E', 'E', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'E', 'E', 'E', 'E', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'E', 'E', 'E', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
};
/* INSTEAD USE game-map.h HEADER */

void updateMap() {

    for(int row = 0; row < LEVEL_ROW_SIZE; row++) {
        for(int col = 0; col < LEVEL_COL_SIZE; col++) {
            if(gameMap[row][col] == 'R') {
                if(gameMap[row+1][col] == 'E') {
                    gameMap[row][col] = 'E';
                    gameMap[row+1][col] = 'B';
                    

                } else if(gameMap[row+1][col] == 'R') {
                    if(gameMap[row+1][col+1] == 'E') {
                        gameMap[row][col] = 'E';
                        gameMap[row+1][col+1] = 'B';

                    } else if(gameMap[row+1][col-1] == 'E') {
                        gameMap[row][col] = 'E';
                        gameMap[row+1][col-1] = 'B';

                    }

                }


            }

        }

    }

    for(int row = 0; row < LEVEL_ROW_SIZE; row++) {
        for(int col = 0; col < LEVEL_COL_SIZE; col++) {
            if(gameMap[row][col] == 'B') {
                gameMap[row][col] = 'R';

            }

        }

    }

}

void renderMap() {
    SDL_RenderClear(gRenderer);
    //i specifies the Y coordinate of render pixel
    for(int i = 0; i < LEVEL_ROW_SIZE; i++) {
        //j specifies the X coordinate of render pixel
        for(int j = 0; j < LEVEL_COL_SIZE; j++) {
            tile_t currentTile = createTile(i, j);
            tile_t cam = createTile(camera.row, camera.col);
            int diffX = (gameWindow.width / 2) - cam.center_x; 
            int diffY = (gameWindow.height / 2) - cam.center_y;
            
            if(isInsideCamera(gameWindow, camera, i, j)) {
                switch (gameMap[i][j]) {
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

    if(gameMap[x][y] == 'W') return false;

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

    playerTexture = loadPioTexture("../src-imgs/playerTexture.png", gRenderer);
    resizePioTexture(&playerTexture, TILE_WIDTH, TILE_HEIGHT);
    if(playerTexture.texture == NULL) {
        printf("Failed to load playerTexture image!\n");
        success = false;

    }

    emptyTexture = loadPioTexture("../src-imgs/emptyTexture.png", gRenderer);
    resizePioTexture(&emptyTexture, TILE_WIDTH, TILE_HEIGHT);
    if(emptyTexture.texture == NULL) {
        printf("Failed to load emptyTexture image!\n");
        success = false;

    }

    dirtTexture = loadPioTexture("../src-imgs/dirtTexture.png", gRenderer);
    resizePioTexture(&dirtTexture, TILE_WIDTH, TILE_HEIGHT);
    if(dirtTexture.texture == NULL) {
        printf("Failed to load dirtTexture image!\n");
        success = false;

    }

    borderTexture = loadPioTexture("../src-imgs/borderTexture.png", gRenderer);
    resizePioTexture(&borderTexture, TILE_WIDTH, TILE_HEIGHT);
    if(borderTexture.texture == NULL) {
        printf("Failed to load borderTexture image!\n");
        success = false;

    }

    rockTexture = loadPioTexture("../src-imgs/rockTexture.png", gRenderer);
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
                                gameMap[miner.row][miner.col] = 'E';
                                gameMap[miner.row-1][miner.col] = 'P';
                                miner.row--;    
                            }
                            
                            break;
                        case SDLK_DOWN:
                            if(isValidCellToMove(miner.row+1, miner.col)) {
                                gameMap[miner.row][miner.col] = 'E';
                                gameMap[miner.row+1][miner.col] = 'P';
                                miner.row++;    
                            }
                            
                            break;
                        case SDLK_LEFT:
                            if(isValidCellToMove(miner.row, miner.col-1)) {
                                gameMap[miner.row][miner.col] = 'E';
                                gameMap[miner.row][miner.col-1] = 'P';
                                miner.col--;    
                            }
                            
                            break;
                        case SDLK_RIGHT:
                            if(isValidCellToMove(miner.row, miner.col+1)) {
                                gameMap[miner.row][miner.col] = 'E';
                                gameMap[miner.row][miner.col+1] = 'P';
                                miner.col++;    
                            }
                            
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

                if(SDL_GetTicks() % 100 == 0) {
                    updateMap();    
                }

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
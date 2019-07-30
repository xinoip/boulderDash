#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "./pio-texture.h"
#include "./pio-text-font.h"

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

const char *TITLE = "pioGame";
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

typedef enum {false, true} bool;

pioTexture_t playerTexture;
pioTexture_t emptyTexture;
pioTexture_t dirtTexture;
pioTexture_t borderTexture;
pioTexture_t rockTexture;

SDL_Window *gWindow = NULL;
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

/* MUST REFACTOR THESE */
const int CAM_ROW_OFF = SCREEN_WIDTH / TILE_WIDTH;
const int CAM_COL_OFF = SCREEN_HEIGHT / TILE_HEIGHT;
int CAM_ROW = 1;
int CAM_COL = 1;

int playerPosRow = 2;
int playerPosCol = 3;

bool isInsideCamera(int row, int col) {

    int leftCol = CAM_COL - CAM_COL_OFF;
    int rightCol = CAM_COL + CAM_COL_OFF;
    int upRow = CAM_ROW - CAM_ROW_OFF;
    int downRow = CAM_ROW + CAM_ROW_OFF;

    return (row >= upRow && row <= downRow && col >= leftCol && col <= rightCol);  

}

typedef struct Tile {
    int row, col;
    int center_x, center_y;
} tile_t;

tile_t createTile(int row, int col) {

    tile_t rtr;
    rtr.row = row;
    rtr.col = col;

    rtr.center_x = (col * TILE_WIDTH) + (TILE_WIDTH / 2);
    rtr.center_y = (row * TILE_HEIGHT) + (TILE_HEIGHT / 2);

    return rtr;

}

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
            tile_t cam = createTile(CAM_ROW, CAM_COL);
            int diffX = (SCREEN_WIDTH / 2) - cam.center_x; 
            int diffY = (SCREEN_HEIGHT / 2) - cam.center_y;
            
            if(isInsideCamera(i, j)) {
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

            int counterMe = 1;

            while(!quit) {
                while(SDL_PollEvent(&e) != 0) {
                    if(e.type == SDL_QUIT) {
                        quit = true;

                    } else if(e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                        case SDLK_UP:
                            if(isValidCellToMove(playerPosRow-1, playerPosCol)) {
                                gameMap[playerPosRow][playerPosCol] = 'E';
                                gameMap[playerPosRow-1][playerPosCol] = 'P';
                                playerPosRow--;    
                            }
                            
                            break;
                        case SDLK_DOWN:
                            if(isValidCellToMove(playerPosRow+1, playerPosCol)) {
                                gameMap[playerPosRow][playerPosCol] = 'E';
                                gameMap[playerPosRow+1][playerPosCol] = 'P';
                                playerPosRow++;    
                            }
                            
                            break;
                        case SDLK_LEFT:
                            if(isValidCellToMove(playerPosRow, playerPosCol-1)) {
                                gameMap[playerPosRow][playerPosCol] = 'E';
                                gameMap[playerPosRow][playerPosCol-1] = 'P';
                                playerPosCol--;    
                            }
                            
                            break;
                        case SDLK_RIGHT:
                            if(isValidCellToMove(playerPosRow, playerPosCol+1)) {
                                gameMap[playerPosRow][playerPosCol] = 'E';
                                gameMap[playerPosRow][playerPosCol+1] = 'P';
                                playerPosCol++;    
                            }
                            
                            break;
                        
                        default:
                            break;
                        }

                    }

                }

                if(SDL_GetTicks() % 100 == 0) {
                    updateMap();    
                }

                CAM_COL = playerPosCol;
                CAM_ROW = playerPosRow;

                renderMap();

                SDL_RenderPresent(gRenderer);

            }

        }
        
    }

    closeAll();

    return 0;

}
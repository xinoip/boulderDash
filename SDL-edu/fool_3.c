#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

/* Constants for window properties */
const char *TITLE = "pioGame";
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
/* Constants for window properties */

/* Constants for renderer default background colors */
const Uint8 BG_R = 0x00;
const Uint8 BG_G = 0x00;
const Uint8 BG_B = 0x00;
const Uint8 BG_A = 0x00;
/* Constants for renderer default background colors */

/* Constants for tiled map */
#define TILE_WIDTH 64
#define TILE_HEIGHT 64
/* Constants for tiled map */

/* Basic boolean implementation */
typedef enum {false, true} bool;
/* Basic boolean implementation */

/* Main texture object for all textures */
typedef struct PioTexture {
    int width, height;
    SDL_Texture *texture;
} pioTexture_t;

//Creates pioTexture with given path and renderer
pioTexture_t loadPioTexture(char *path, SDL_Renderer *renderer) {

    pioTexture_t rtr;
    rtr.texture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());

    } else {
        rtr.texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(rtr.texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());

        } else {
            rtr.width = loadedSurface->w;
            rtr.height = loadedSurface->h;

        }

        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;

    }

    return rtr;

}

//Renders given texture at (x,y) using the given renderer
//(x,y) will be center of the image
void renderPioTexture(pioTexture_t p, int x, int y, SDL_Renderer *renderer) {

    SDL_Rect renderQuad = {x-(p.width/2), y-(p.height/2), p.width, p.height};

    SDL_RenderCopy(renderer, p.texture, NULL, &renderQuad);

}

//Frees given pioTexture from memory
void destroyPioTexture(pioTexture_t *p) {

    SDL_DestroyTexture(p->texture);
    p->texture = NULL;

}

//Resizes given pioTexture with given newWidth and newHeight values
//Streches or minimizes the texture
void resizePioTexture(pioTexture_t *p, int newWidth, int newHeight) {

    p->width = newWidth;
    p->height = newHeight;

}
/* Main texture object for all textures */

/* Main game textures */
pioTexture_t playerTexture;
pioTexture_t emptyTexture;
pioTexture_t dirtTexture;
pioTexture_t borderTexture;
/* Main game textures */

/* Game launch phases */
bool init();
bool loadMedia();
void closeAll();
/* Game launch phases */

/* Main window and renderer */
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
/* Main window and renderer */

/* Map debugging */
const LEVEL_ROW_SIZE = 13;
const LEVEL_COL_SIZE = 17;
char gameMap[13][17] = {
    {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
    {'W', 'P', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'D', 'W'},
    {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
};

const int CAM_ROW_OFF = SCREEN_WIDTH / TILE_WIDTH;
const int CAM_COL_OFF = SCREEN_HEIGHT / TILE_HEIGHT;
int CAM_ROW = 1;
int CAM_COL = 1;

int playerPosRow = 1;
int playerPosCol = 1;

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
                default:
                    break;
                }
            }

        }
    }

}

bool isValidCellToMove(const int x, const int y) {

    if(gameMap[x][y] == 'W') return false;

    return true;
}

/* Map debugging */

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
                SDL_SetRenderDrawColor(gRenderer, BG_R, BG_G, BG_G, BG_A);

                int imgFlags = IMG_INIT_PNG;
                if(!(IMG_Init( imgFlags ) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;

				}

            }

        }

    }

    return success;

}

bool loadMedia() {

    bool success = true;

    playerTexture = loadPioTexture("./src-imgs/playerTexture.png", gRenderer);
    resizePioTexture(&playerTexture, TILE_WIDTH, TILE_HEIGHT);
    if(playerTexture.texture == NULL) {
        printf("Failed to load playerTexture image!\n");
        success = false;

    }

    emptyTexture = loadPioTexture("./src-imgs/emptyTexture.png", gRenderer);
    resizePioTexture(&emptyTexture, TILE_WIDTH, TILE_HEIGHT);
    if(emptyTexture.texture == NULL) {
        printf("Failed to load emptyTexture image!\n");
        success = false;

    }

    dirtTexture = loadPioTexture("./src-imgs/dirtTexture.png", gRenderer);
    resizePioTexture(&dirtTexture, TILE_WIDTH, TILE_HEIGHT);
    if(dirtTexture.texture == NULL) {
        printf("Failed to load dirtTexture image!\n");
        success = false;

    }

    borderTexture = loadPioTexture("./src-imgs/borderTexture.png", gRenderer);
    resizePioTexture(&borderTexture, TILE_WIDTH, TILE_HEIGHT);
    if(borderTexture.texture == NULL) {
        printf("Failed to load borderTexture image!\n");
        success = false;

    }

    return success;

}

void closeAll() {

    destroyPioTexture(&dirtTexture);
    destroyPioTexture(&borderTexture);
    destroyPioTexture(&playerTexture);
    destroyPioTexture(&emptyTexture);

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

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

                CAM_COL = playerPosCol;
                CAM_ROW = playerPosRow;

                renderMap();

                SDL_RenderPresent(gRenderer);

                //quit = true;
                
            }

        }
        
    }

    closeAll();

    return 0;

}
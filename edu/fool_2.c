#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const char *TITLE = "pioGame";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef enum {false, true} bool;

#define XSIZE 5
#define YSIZE 5
#define SCALER 100

int playerPosX = 1;
int playerPosY = 1;

char gameMap[XSIZE][YSIZE] = {
    {'W', 'W', 'W', 'W', 'W'},
    {'W', 'P', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'W'},
    {'W', 'D', 'D', 'D', 'W'},
    {'W', 'W', 'W', 'W', 'W'}
};

void printGameMapToConsole() {
    for(int i = 0; i < XSIZE; i++) {
        for(int j = 0; j < YSIZE; j++) {
            printf("%c", gameMap[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

bool init();
bool loadMedia();
void closeAll();

SDL_Texture * loadTexture(char *path);

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Texture *playerTexture = NULL;
SDL_Texture *dirtTexture = NULL;
SDL_Texture *wallTexture = NULL;
SDL_Texture *emptyTexture = NULL;

void renderFrame() {
    for(int i = 0; i < XSIZE; i++) {
        for(int j = 0; j < YSIZE; j++) {
            SDL_Rect currentCell;
            currentCell.x = j * SCALER;
            currentCell.y = i * SCALER;
            currentCell.w = 128;
            currentCell.h = 128;
            SDL_RenderSetViewport(gRenderer, &currentCell);
            switch (gameMap[i][j]) {
            case 'P':
                SDL_RenderCopy(gRenderer, playerTexture, NULL, NULL);
                break;
            case 'W':
                SDL_RenderCopy(gRenderer, wallTexture, NULL, NULL);
                break;    
            case 'D':
                SDL_RenderCopy(gRenderer, dirtTexture, NULL, NULL);
                break;    
            case 'E':
                SDL_RenderCopy(gRenderer, emptyTexture, NULL, NULL);
                break;
            }  
        }
    }
    
    SDL_RenderPresent(gRenderer);

}

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
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;

				}

            }

        }

    }

    return success;

}

bool loadMedia() {

    bool success = true;

    playerTexture = loadTexture("./src-imgs/playerTexture.png");
    if(playerTexture == NULL) {
        printf("Failed to load playerTexture image!\n");
        success = false;

    }

    dirtTexture = loadTexture("./src-imgs/dirtTexture.png");
    if(dirtTexture == NULL) {
        printf("Failed to load dirtTexture image!\n");
        success = false;

    }

    wallTexture = loadTexture("./src-imgs/wallTexture.png");
    if(wallTexture == NULL) {
        printf("Failed to load wallTexture image!\n");
        success = false;

    }

    emptyTexture = loadTexture("./src-imgs/emptyTexture.png");
    if(emptyTexture == NULL) {
        printf("Failed to load emptyTexture image!\n");
        success = false;

    }

    return success;

}

void closeAll() {

    SDL_DestroyTexture(playerTexture);
    playerTexture = NULL;
    SDL_DestroyTexture(dirtTexture);
    dirtTexture = NULL;
    SDL_DestroyTexture(wallTexture);
    wallTexture = NULL;
    SDL_DestroyTexture(emptyTexture);
    emptyTexture = NULL;


    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();

}

SDL_Texture * loadTexture(char *path) {

    SDL_Texture *newTexture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());

    } else {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(newTexture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());

        }

        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;

    }

    return newTexture;

}

bool isValidCellToMove(const int x, const int y) {

    if(gameMap[x][y] == 'W') return false;

    return true;
}

int main(int argc, char *args[]) {

    printGameMapToConsole();

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
                            printf("up\n");
                            if(isValidCellToMove(playerPosX-1, playerPosY)) {
                                gameMap[playerPosX][playerPosY] = 'E';
                                gameMap[playerPosX-1][playerPosY] = 'P';
                                playerPosX--;
                            }
                            break;
                        case SDLK_DOWN:
                            printf("down\n");
                            if(isValidCellToMove(playerPosX+1, playerPosY)) {
                                gameMap[playerPosX][playerPosY] = 'E';
                                gameMap[playerPosX+1][playerPosY] = 'P';
                                playerPosX++;
                            }
                            break;
                        case SDLK_LEFT:
                            printf("left\n");
                            if(isValidCellToMove(playerPosX, playerPosY-1)) {
                                gameMap[playerPosX][playerPosY] = 'E';
                                gameMap[playerPosX][playerPosY-1] = 'P';
                                playerPosY--;
                            }
                            break;
                        case SDLK_RIGHT:
                            printf("right\n");
                            if(isValidCellToMove(playerPosX, playerPosY+1)) {
                                gameMap[playerPosX][playerPosY] = 'E';
                                gameMap[playerPosX][playerPosY+1] = 'P';
                                playerPosY++;
                            }
                            break;
                        default:
                            printf("other\n");
                            break;
                        }

                    }

                }

                renderFrame();

            }

        }
        
    }

    closeAll();

    return 0;

}
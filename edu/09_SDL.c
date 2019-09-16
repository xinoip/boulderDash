#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const char *TITLE = "pioGame";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef enum {false, true} bool;

bool init();
bool loadMedia();
void closeAll();

SDL_Texture * loadTexture(char *path);

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Texture *gTexture = NULL;

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

    gTexture = loadTexture("./src-imgs/viewport.png");
    if(gTexture == NULL) {
        printf("Failed to load texture image!\n");
        success = false;

    }

    return success;

}

void closeAll() {

    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

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

                SDL_Rect topLeftViewPort;
                topLeftViewPort.x = 0;
                topLeftViewPort.y = 0;
                topLeftViewPort.w = SCREEN_WIDTH / 2;
                topLeftViewPort.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport(gRenderer, &topLeftViewPort);
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);
                
                SDL_Rect bottomViewPort;
                bottomViewPort.x = 0;
                bottomViewPort.y = SCREEN_HEIGHT / 2;
                bottomViewPort.w = SCREEN_WIDTH;
                bottomViewPort.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport(gRenderer, &bottomViewPort);
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

                SDL_Rect topRightViewPort;
                topRightViewPort.x = SCREEN_WIDTH / 2;
                topRightViewPort.y = 0;
                topRightViewPort.w = SCREEN_WIDTH / 2;
                topRightViewPort.h = SCREEN_HEIGHT / 2;
                SDL_RenderSetViewport(gRenderer, &topRightViewPort);
                SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

                SDL_RenderPresent(gRenderer);

            }

        }
        
    }

    closeAll();

    return 0;

}
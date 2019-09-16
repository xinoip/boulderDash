#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const char *TITLE = "pioGame";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int posX = 0;
int posY = 0;

typedef enum {false, true} bool;

typedef struct PioTexture {
    SDL_Texture *texture;
    int mWidth, mHeight;
} pioTexture_t;

bool init();
bool loadMedia();
void closeAll();

pioTexture_t loadTextureFromFile(char *path);

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

pioTexture_t pTexture;

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

    pTexture = loadTextureFromFile("./src-imgs/playerTexture.png");
    if(pTexture.texture == NULL) {
        printf("Failed to load texture image!\n");
        success = false;

    }

    return success;

}

void destroyPioTexture(pioTexture_t p) {

    SDL_DestroyTexture(p.texture);
    p.texture = NULL;

}

void closeAll() {

    destroyPioTexture(pTexture);

    SDL_DestroyRenderer(gRenderer);
    gRenderer = NULL;

    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    IMG_Quit();
    SDL_Quit();

}

pioTexture_t loadTextureFromFile(char *path) {

    pioTexture_t rtr;

    rtr.texture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());

    } else {
        rtr.texture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if(rtr.texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());

        } else {
            rtr.mWidth = loadedSurface->w;
            rtr.mHeight = loadedSurface->h;

        }

        SDL_FreeSurface(loadedSurface);
        loadedSurface = NULL;

    }

    return rtr;

}

void renderPioTexture(pioTexture_t p, int x, int y, SDL_Renderer *renderer) {

    SDL_Rect renderQuad = {x, y, p.mWidth, p.mHeight};

    SDL_RenderCopy(renderer, p.texture, NULL, &renderQuad);

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

    const int DIFF_MOVE = 5;

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
						switch(e.key.keysym.sym) {
							case SDLK_UP:
							printf("up\n");
							posY-= DIFF_MOVE;
							break;
							case SDLK_DOWN:
							printf("down\n");
							posY+= DIFF_MOVE;
							break;
							case SDLK_LEFT:
							printf("left\n");
							posX-= DIFF_MOVE;
							break;
							case SDLK_RIGHT:
							printf("right\n");
							posX+= DIFF_MOVE;
							break;
							default:
							printf("other\n");
							break;

						}
					}

                }

                SDL_RenderClear(gRenderer);

                renderPioTexture(pTexture, posX, posY, gRenderer);

                SDL_RenderPresent(gRenderer);

            }

        }
        
    }

    closeAll();

    return 0;

}
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

const char *TITLE = "pioGame";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

typedef enum {false, true} bool;

typedef struct PioTexture {
    int width, height;
    SDL_Texture *texture;
} pioTexture_t;

typedef struct PioTextFont {
    char *text;
    SDL_Color color;
    TTF_Font *font;
    pioTexture_t texture;
} pioTextFont_t;

bool init();
bool loadMedia();
void closeAll();

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

TTF_Font *gFont = NULL;

pioTextFont_t mainText;

pioTextFont_t loadPioTextFont(char *text, SDL_Color color, TTF_Font *font) {

    pioTextFont_t rtr;
    strcpy(rtr.text, text);
    rtr.color = color;
    rtr.font = font;

    SDL_Surface *textSurface = TTF_RenderText_Solid(rtr.font, rtr.text, rtr.color);
    if(textSurface == NULL) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());

    } else {
        rtr.texture.texture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if(rtr.texture.texture == NULL) {
            printf("Unable to create texture from rendered text! SDL Error: %s", SDL_GetError());

        } else {
            rtr.texture.width = textSurface->w;
            rtr.texture.height = textSurface->h;

        }

        SDL_FreeSurface(textSurface);
        textSurface = NULL;
    }

    return rtr;

}

//Frees given pioTexture from memory
void destroyPioTexture(pioTexture_t *p) {

    SDL_DestroyTexture(p->texture);
    p->texture = NULL;

}

void destroyPioTextFont(pioTextFont_t text) {

    text.text = NULL;
    destroyPioTexture(&(text.texture));

}

void renderPioTexture(pioTexture_t p, int x, int y, SDL_Renderer *renderer) {

    SDL_Rect renderQuad = {x-(p.width/2), y-(p.height/2), p.width, p.height};

    SDL_RenderCopy(renderer, p.texture, NULL, &renderQuad);

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

    gFont = TTF_OpenFont("./src-imgs/lazy.ttf", 28);
    if(gFont == NULL) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;

    }

    SDL_Color textColor = {0, 0, 0};
    mainText = loadPioTextFont("abc", textColor, gFont);

    return success;

}

void closeAll() {

    destroyPioTextFont(mainText);

    TTF_CloseFont(gFont);
    gFont = NULL;

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

                    }

                }

                //printf("text: %s, width: %d, height: %d\n", mainText.text, mainText.texture.width, mainText.texture.height);

                SDL_RenderClear(gRenderer);

                renderPioTexture(mainText.texture, 50, 50, gRenderer);

                SDL_RenderPresent(gRenderer);

                //quit = true;

            }

        }
        
    }

    closeAll();

    return 0;

}
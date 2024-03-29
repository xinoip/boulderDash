#include "./pio-texture.h"

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

void renderPioTextureCornered(pioTexture_t p, int x, int y, SDL_Renderer *renderer) {

    SDL_Rect renderQuad = {x, y, p.width, p.height};

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

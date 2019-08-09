#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include "./wrappers/pio-texture.h"
#include "./wrappers/pio-window.h"
#include "./objects/level.h"
#include "./objects/camera.h"
#include "./objects/tile.h"

void renderMap(level_t level, camera_t camera, pioWindow_t window, SDL_Renderer *renderer);

bool loadMedia(SDL_Renderer *renderer);

void closeMedia();

#endif
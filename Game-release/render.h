#ifndef RENDER_H
#define RENDER_H

#include "./include/SDL.h"
#include "./include/SDL_ttf.h"
#include "./include/SDL_mixer.h"
#include "./base.h"
#include "./wrappers/pio-texture.h"
#include "./wrappers/pio-window.h"
#include "./wrappers/pio-text-font.h"
#include "./objects/level.h"
#include "./objects/camera.h"
#include "./objects/tile.h"

void renderMap(level_t level, camera_t camera, pioWindow_t window, SDL_Renderer *renderer);

bool loadMedia(SDL_Renderer *renderer);

void closeMedia();

void renderGameBar(level_t level, pioWindow_t window, SDL_Renderer *renderer, bool isPaused);

void updateGameBar(level_t level, SDL_Renderer *renderer, int lives);

void renderOnDeath(level_t level, camera_t camera, pioWindow_t window, SDL_Renderer *renderer);

void startMusic();
void playBoulderFall();
void playDiamondCollect();
void playDirtRemove();
void startMusic10(); 
void startMusic30();
void startMusic60();
void stopMusic();

#endif
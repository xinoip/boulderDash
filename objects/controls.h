#ifndef CONTROLS_H
#define CONTROLS_H

typedef struct Miner miner_t;

#include "../libs/include/SDL.h"
#include "../base.h"
#include "./level.h"
#include "./miner.h"
#include "./camera.h"

bool isValidCellToMove(level_t level, int row, int col);

void updateMap(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer);

void updateWater(level_t *level);

#endif
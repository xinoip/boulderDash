#ifndef CONTROLS_H
#define CONTROLS_H

typedef struct Miner miner_t;

#include <SDL2/SDL.h>
#include "../base.h"
#include "./level.h"
#include "./miner.h"

bool isValidCellToMove(level_t level, int row, int col);

void updateMap(level_t *level, miner_t *miner);

void updateWater(level_t *level);

#endif
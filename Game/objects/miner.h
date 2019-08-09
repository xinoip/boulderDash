#ifndef MINER_H
#define MINER_H

#include <SDL2/SDL.h>
#include "./level.h"
#include "./controls.h"

typedef struct Miner {
    int row, col;
} miner_t;

miner_t createMiner(int row, int col);

void updateMiner(miner_t *miner, int newRow, int newCol);

void moveMiner(miner_t *miner, level_t *level, SDL_Event event);

#endif
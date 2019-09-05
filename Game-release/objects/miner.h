#ifndef MINER_H
#define MINER_H

typedef struct Miner miner_t;

#include "../include/SDL.h"
#include <stdlib.h>
#include "../base.h"
#include "./level.h"
#include "./controls.h"
#include "../render.h"

typedef struct Miner {
    int row, col;
    int level;
    int lives;
} miner_t;

typedef enum MoveDirection {
    upDir, downDir, leftDir, rightDir
} moveDirection_t;

miner_t createMiner(int row, int col);

void updateMiner(miner_t *miner, int newRow, int newCol);

void moveMiner(miner_t *miner, level_t *level, SDL_Event event);

//pass direction as parameter too for rock moving
void processMove(level_t *level, miner_t *miner, int newRow, int newCol, moveDirection_t dir);

#endif
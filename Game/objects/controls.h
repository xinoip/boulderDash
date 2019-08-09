#ifndef CONTROLS_H
#define CONTROLS_H

#include <SDL2/SDL.h>
#include "../base.h"
#include "./level.h"

bool isValidCellToMove(level_t level, int row, int col);

void updateRocks(level_t *level);

void updateMap(level_t *level);

#endif
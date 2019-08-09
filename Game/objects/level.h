#ifndef LEVEL_H
#define LEVEL_H

#include "../base.h"

typedef struct Level {
    char **map;
    int row, col;
    int startMinerRow, startMinerCol;
} level_t;

level_t createLevel(int row, int col, int minerRow, int minerCol);

void fillLevel(level_t *level, char *filename);

void printLevel(level_t level);

#endif
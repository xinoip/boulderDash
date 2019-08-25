#ifndef LEVEL_H
#define LEVEL_H

#include "../base.h"

typedef struct Level {
    char **map;
    int row, col;
    int startMinerRow, startMinerCol;
    int diamondCount;
    int timeLimit;
    int waterMs;
    char *name;
} level_t;

level_t createLevel(int row, int col, int minerRow, int minerCol, int diamondCount, char *levelName, int timeLimit, int waterMs);

void fillLevel(level_t *level, char *filename);

void printLevel(level_t level);

#endif
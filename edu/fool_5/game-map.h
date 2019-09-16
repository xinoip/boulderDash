#ifndef GAME_MAP_H
#define GAME_MAP_H

/* should be getting these from the map file */
#define ROW 10
#define COL 10

typedef struct GameMap {
    char map[ROW][COL]; /* should be dynamicly allocated */
    int row, col;
} gameMap_t;

gameMap_t * loadGameMap(/* will have a path parameter */);

#endif
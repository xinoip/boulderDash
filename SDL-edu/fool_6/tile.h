#ifndef TILE_H
#define TILE_H

typedef struct Tile {
    int row, col;
    int center_x, center_y;
} tile_t;

tile_t createTile(int row, int col);

#endif
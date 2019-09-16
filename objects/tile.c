#include "tile.h"
#include "../base.h"

tile_t createTile(int row, int col) {

    tile_t rtr;
    rtr.row = row;
    rtr.col = col;

    rtr.center_x = (col * TILE_WIDTH) + (TILE_WIDTH / 2);
    rtr.center_y = (row * TILE_HEIGHT) + (TILE_HEIGHT / 2);

    return rtr;

}
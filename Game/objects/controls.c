#include "./controls.h"

bool isValidCellToMove(level_t level, int row, int col) {

    if(level.map[row][col] == 'W') return false;

    return true;

}

void updateMap(level_t *level) {
    updateRocks(level);
}

void updateRocks(level_t *level) {
    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == 'R') {
                if(level->map[row+1][col] == 'E') {
                    level->map[row][col] = 'E';
                    level->map[row+1][col] = 'B';
                    

                } else if(level->map[row+1][col] == 'R') {
                    if(level->map[row+1][col+1] == 'E') {
                        level->map[row][col] = 'E';
                        level->map[row+1][col+1] = 'B';

                    } else if(level->map[row+1][col-1] == 'E') {
                        level->map[row][col] = 'E';
                        level->map[row+1][col-1] = 'B';

                    }

                }


            }

        }

    }

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == 'B') {
                level->map[row][col] = 'R';

            }

        }

    }
}
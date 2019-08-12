#include "./controls.h"

bool isValidCellToMove(level_t level, int row, int col) {

    if(level.map[row][col] == 'W') return false;

    return true;

}

void updateMap(level_t *level) {
    //updateRocks(level);
    updateFallingObjects(level);
}

void updateFallingObjects(level_t *level) {
    char fallingSymbol;
    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == 'R' || level->map[row][col] == 'X') { //if fallable object
                if(level->map[row][col] == 'R') fallingSymbol = 'B';
                if(level->map[row][col] == 'X') fallingSymbol = 'T';

                if(level->map[row+1][col] == 'E') { //free fall
                    level->map[row][col] = 'E';
                    level->map[row+1][col] = fallingSymbol;
                    
                } else if(level->map[row+1][col] == 'R' || level->map[row+1][col] == 'X') { //crash into another fallable object
                    if(level->map[row][col+1] == 'E' && level->map[row+1][col+1] == 'E') {
                        level->map[row][col] = 'E';
                        level->map[row+1][col+1] = fallingSymbol;

                    } else if(level->map[row][col-1] == 'E' && level->map[row+1][col-1] == 'E') {
                        level->map[row][col] = 'E';
                        level->map[row+1][col-1] = fallingSymbol;

                    }

                }

            }

        }

    }

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == 'B') { //rocks
                level->map[row][col] = 'R';

            }
            if(level->map[row][col] == 'T') { //diamonds
                level->map[row][col] = 'X';

            }

        }

    }
}

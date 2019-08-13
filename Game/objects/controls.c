#include "./controls.h"

bool isValidCellToMove(level_t level, int row, int col) {

    if(level.map[row][col] == 'W') return false;

    return true;

}

void updateMap(level_t *level) {
    //updateRocks(level);
    updateFallingObjects(level);
    updateSpiders(level);
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

                } else if(level->map[row][col] == 'R' && level->map[row+1][col] == 'S') { //crashing spiderrssss
                    level->map[row][col] = 'E';
                    level->map[row+1][col] = 'B';
                    generateDiaOnDeath(level, row+1, col);

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

void updateSpiders(level_t *level) {

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {

            if(level->map[row][col] == 'S') {
                if(level->map[row][col+1] == 'E') { //sag bos ise
                    //printf("saga gitti\n");
                    level->map[row][col] = 'E';
                    level->map[row][col+1] = 'M';

                } else if(level->map[row+1][col] == 'E') { //asagisi bos ise
                    //printf("asagi gitti\n");
                    level->map[row][col] = 'E';
                    level->map[row+1][col] = 'M';


                } else if(level->map[row][col-1] == 'E') { //sol bos ise
                    //printf("sola gitti\n");
                    level->map[row][col] = 'E';
                    level->map[row][col-1] = 'M';

                } else if(level->map[row-1][col] == 'E') { //yukari bos ise
                    //printf("yukari gitti\n");
                    level->map[row][col] = 'E';
                    level->map[row-1][col] = 'M';

                } else {
                    printf("ERROR!\n");

                }

            }

        }

    }

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == 'M') { //moving spiders
                level->map[row][col] = 'S';

            }

        }

    }

}

void generateDiaOnDeath(level_t *level, int row, int col) {

    level->map[row][col] = 'X';

    if(level->map[row][col+1] != 'W' && level->map[row][col+1] != 'P') { //sag
        level->map[row][col+1] = 'X';
    } 
    if(level->map[row][col-1] != 'W' && level->map[row][col-1] != 'P') { //sol
        level->map[row][col-1] = 'X';
    }
    if(level->map[row-1][col] != 'W' && level->map[row-1][col] != 'P') { //yukari
        level->map[row-1][col] = 'X';
    } 
    if(level->map[row+1][col] != 'W' && level->map[row+1][col] != 'P') { //asagi
        level->map[row+1][col] = 'X';
    }
    if(level->map[row-1][col+1] != 'W' && level->map[row-1][col+1] != 'P') { //sag-yukari
        level->map[row-1][col+1] = 'X';
    } 
    if(level->map[row-1][col-1] != 'W' && level->map[row-1][col-1] != 'P') { //sol-yukari
        level->map[row-1][col-1] = 'X';
    }
    if(level->map[row+1][col+1] != 'W' && level->map[row+1][col+1] != 'P') { //sag-asagi
        level->map[row+1][col+1] = 'X';
    } 
    if(level->map[row+1][col-1] != 'W' && level->map[row+1][col-1] != 'P') { //sol-asagi
        level->map[row+1][col-1] = 'X';
    }

}
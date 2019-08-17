#include "./controls.h"

bool isValidCellToMove(level_t level, int row, int col) {

    if(level.map[row][col] == 'W') return false;

    return true;

}

void updateMap(level_t *level, miner_t *miner) {
    updateFallingObjects(level, miner);
    updateSpiders(level, miner);
}

void updateFallingObjects(level_t *level, miner_t *miner) {
    char fallingSymbol;
    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == 'R' || level->map[row][col] == 'X') { // If fall-able

                // Determine falling symbol
                if(level->map[row][col] == 'R') fallingSymbol = 'B';
                if(level->map[row][col] == 'X') fallingSymbol = 'T';

                if(level->map[row+1][col] == 'E') { // Free fall
                    level->map[row][col] = 'E';
                    level->map[row+1][col] = fallingSymbol;
                    
                } else if(level->map[row+1][col] == 'R' || level->map[row+1][col] == 'X') { // Crash into another fallable object
                    if(level->map[row][col+1] == 'E' && level->map[row+1][col+1] == 'E') {  // Fall to right
                        level->map[row][col] = 'E';
                        level->map[row+1][col+1] = fallingSymbol;

                    } else if(level->map[row][col-1] == 'E' && level->map[row+1][col-1] == 'E') { // Fall to left
                        level->map[row][col] = 'E';
                        level->map[row+1][col-1] = fallingSymbol;

                    }

                } else if(level->map[row][col] == 'R' && level->map[row+1][col] == 'S') { // Crashing spiders
                    level->map[row][col] = 'E';
                    level->map[row+1][col] = 'B';
                    generateDiaOnDeath(level, row+1, col);

                } 

            }

        }

    }

    // Convert falling objects to stationary objects
    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == 'B') { // Falling rocks
                if(level->map[row+1][col] == 'P') { // Crashin player
                    level->map[row][col] = 'E';
                    level->map[row+1][col] = 'R';
                    killMiner(level, miner);

                } else { // Just convert to stationary
                    level->map[row][col] = 'R';

                }
                

            }

            if(level->map[row][col] == 'T') { // Falling diamonds
                level->map[row][col] = 'X';

            }

        }

    }
}

void updateSpiders(level_t *level, miner_t *miner) {

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {

            if(level->map[row][col] == 'S') {

                if(level->map[row][col+1] == 'P') { // Kill player right
                    level->map[row][col] = 'E';
                    level->map[row][col+1] = 'M';
                    killMiner(level, miner);

                } else if(level->map[row+1][col] == 'P') { // Kill player bottom
                    level->map[row][col] = 'E';
                    level->map[row+1][col] = 'M';
                    killMiner(level, miner); 

                } else if(level->map[row][col-1] == 'P') { // Kill player left
                    level->map[row][col] = 'E';
                    level->map[row][col-1] = 'M';
                    killMiner(level, miner);

                } else if(level->map[row-1][col] == 'P') { // Kill player up
                    level->map[row][col] = 'E';
                    level->map[row-1][col] = 'M';
                    killMiner(level, miner);

                } else {
                    // No player in range

                    if(level->map[row][col+1] == 'E') { // Right
                        level->map[row][col] = 'E';
                        level->map[row][col+1] = 'M';

                    } else if(level->map[row+1][col] == 'E') { // Bottom
                        level->map[row][col] = 'E';
                        level->map[row+1][col] = 'M';


                    } else if(level->map[row][col-1] == 'E') { // Left
                        level->map[row][col] = 'E';
                        level->map[row][col-1] = 'M';

                    } else if(level->map[row-1][col] == 'E') { // Up
                        level->map[row][col] = 'E';
                        level->map[row-1][col] = 'M';

                    } else {
                        // Spider stuck

                    }

                }
                
            }

        }

    }

    // Convert moving spiders to stationary spiders
    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == 'M') { // Moving spiders
                level->map[row][col] = 'S';

            }

        }

    }

}

void updateMonsters(level_t *level, miner_t *miner, int row, int col) {
    
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

void killMiner(level_t *level, miner_t *miner) {
    printf("YOU DIED\n");
    miner->row = level->startMinerRow;
    miner->col = level->startMinerCol;
    level->map[miner->row][miner->col] = 'P';
}
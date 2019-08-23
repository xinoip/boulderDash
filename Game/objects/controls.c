#include "./controls.h"

bool isValidCellToMove(level_t level, int row, int col) {

    if(level.map[row][col] == borderTile) return false;

    return true;

}

void updateMap(level_t *level, miner_t *miner) {
    updateFallingObjects(level, miner);
    updateSpiders(level, miner);
    updateMonsters(level, miner);
}

void updateFallingObjects(level_t *level, miner_t *miner) {
    char fallingSymbol;
    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == rockTile || level->map[row][col] == diamondTile) { // If fall-able

                // Determine falling symbol
                if(level->map[row][col] == rockTile) fallingSymbol = fallingRockTile;
                if(level->map[row][col] == diamondTile) fallingSymbol = fallingDiamondTile;

                if(level->map[row+1][col] == emptyTile) { // Free fall
                    level->map[row][col] = emptyTile;
                    level->map[row+1][col] = fallingSymbol;
                    
                } else if(level->map[row+1][col] == rockTile || level->map[row+1][col] == diamondTile) { // Crash into another fallable object
                    if(level->map[row][col+1] == emptyTile && level->map[row+1][col+1] == emptyTile) {  // Fall to right
                        level->map[row][col] = emptyTile;
                        level->map[row+1][col+1] = fallingSymbol;

                    } else if(level->map[row][col-1] == emptyTile && level->map[row+1][col-1] == emptyTile) { // Fall to left
                        level->map[row][col] = emptyTile;
                        level->map[row+1][col-1] = fallingSymbol;

                    }

                } else if(level->map[row][col] == rockTile && level->map[row+1][col] == spiderTile) { // Crashing spiders
                    level->map[row][col] = emptyTile;
                    level->map[row+1][col] = fallingRockTile;
                    generateDiaOnDeath(level, row+1, col);

                } 

            }

        }

    }

    // Convert falling objects to stationary objects
    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == fallingRockTile) { // Falling rocks
                if(level->map[row+1][col] == playerTile) { // Crashin player
                    level->map[row][col] = emptyTile;
                    level->map[row+1][col] = rockTile;
                    killMiner(level, miner);

                } else { // Just convert to stationary
                    level->map[row][col] = rockTile;

                }
                

            }

            if(level->map[row][col] == fallingDiamondTile) { // Falling diamonds
                level->map[row][col] = diamondTile;

            }

        }

    }
}

void updateSpiders(level_t *level, miner_t *miner) {

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {

            if(level->map[row][col] == spiderTile) {

                if(level->map[row][col+1] == playerTile) { // Kill player right
                    level->map[row][col] = emptyTile;
                    level->map[row][col+1] = movingSpiderTile;
                    killMiner(level, miner);

                } else if(level->map[row+1][col] == playerTile) { // Kill player bottom
                    level->map[row][col] = emptyTile;
                    level->map[row+1][col] = movingSpiderTile;
                    killMiner(level, miner); 

                } else if(level->map[row][col-1] == playerTile) { // Kill player left
                    level->map[row][col] = emptyTile;
                    level->map[row][col-1] = movingSpiderTile;
                    killMiner(level, miner);

                } else if(level->map[row-1][col] == playerTile) { // Kill player up
                    level->map[row][col] = emptyTile;
                    level->map[row-1][col] = movingSpiderTile;
                    killMiner(level, miner);

                } else {
                    // No player in range

                    if(level->map[row][col+1] == emptyTile) { // Right
                        level->map[row][col] = emptyTile;
                        level->map[row][col+1] = movingSpiderTile;

                    } else if(level->map[row+1][col] == emptyTile) { // Bottom
                        level->map[row][col] = emptyTile;
                        level->map[row+1][col] = movingSpiderTile;


                    } else if(level->map[row][col-1] == emptyTile) { // Left
                        level->map[row][col] = emptyTile;
                        level->map[row][col-1] = movingSpiderTile;

                    } else if(level->map[row-1][col] == emptyTile) { // Up
                        level->map[row][col] = emptyTile;
                        level->map[row-1][col] = movingSpiderTile;

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
            if(level->map[row][col] == movingSpiderTile) { // Moving spiders
                level->map[row][col] = spiderTile;

            }

        }

    }

}

void updateMonsters(level_t *level, miner_t *miner) {
    
    int targetRow = miner->row;
    int targetCol = miner->col;

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {

            if(level->map[row][col] == monsterTile) {
                // kill miner if possible
                if(level->map[row][col+1] == playerTile || level->map[row][col-1] == playerTile || level->map[row+1][col] == playerTile || level->map[row-1][col] == playerTile) {
                    level->map[row][col] = emptyTile;
                    level->map[miner->row][miner->col] = monsterTile;
                    killMiner(level, miner);

                } else if(targetRow > row) { // miner on bottom
                    if(level->map[row+1][col] == emptyTile) {
                        level->map[row][col] = emptyTile;
                        level->map[row+1][col] = monsterTile;

                    }

                } else if(targetRow < row) { // miner on top
                    if(level->map[row-1][col] == emptyTile) {
                        level->map[row][col] = emptyTile;
                        level->map[row-1][col] = monsterTile;

                    }    

                } else if(targetRow == row) { // miner on same level
                    if(targetCol > col) { // miner on right
                        if(level->map[row][col+1] == emptyTile) {
                            level->map[row][col] = emptyTile;
                            level->map[row][col+1] = monsterTile;

                        }

                    } else if(targetCol < col) { // miner on left
                        if(level->map[row][col-1] == emptyTile) {
                            level->map[row][col] = emptyTile;
                            level->map[row][col-1] = monsterTile;
                            
                        }

                    } else if(targetCol == col) { // miner on same col
                        

                    }

                }

            }

        }
    }

}

void generateDiaOnDeath(level_t *level, int row, int col) {

    level->map[row][col] = diamondTile;

    if(level->map[row][col+1] != borderTile && level->map[row][col+1] != playerTile) { //sag
        level->map[row][col+1] = diamondTile;
    } 
    if(level->map[row][col-1] != borderTile && level->map[row][col-1] != playerTile) { //sol
        level->map[row][col-1] = diamondTile;
    }
    if(level->map[row-1][col] != borderTile && level->map[row-1][col] != playerTile) { //yukari
        level->map[row-1][col] = diamondTile;
    } 
    if(level->map[row+1][col] != borderTile && level->map[row+1][col] != playerTile) { //asagi
        level->map[row+1][col] = diamondTile;
    }
    if(level->map[row-1][col+1] != borderTile && level->map[row-1][col+1] != playerTile) { //sag-yukari
        level->map[row-1][col+1] = diamondTile;
    } 
    if(level->map[row-1][col-1] != borderTile && level->map[row-1][col-1] != playerTile) { //sol-yukari
        level->map[row-1][col-1] = diamondTile;
    }
    if(level->map[row+1][col+1] != borderTile && level->map[row+1][col+1] != playerTile) { //sag-asagi
        level->map[row+1][col+1] = diamondTile;
    } 
    if(level->map[row+1][col-1] != borderTile && level->map[row+1][col-1] != playerTile) { //sol-asagi
        level->map[row+1][col-1] = diamondTile;
    }

}

void killMiner(level_t *level, miner_t *miner) {
    printf("YOU DIED\n");
    miner->row = level->startMinerRow;
    miner->col = level->startMinerCol;
    level->map[miner->row][miner->col] = playerTile;
}
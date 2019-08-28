#include "./controls.h"
#include "../render.h"

bool isValidCellToMove(level_t level, int row, int col) {

    if(level.map[row][col] == borderTile) return false;

    return true;

}

void updateMap(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
    updateFallingObjects(level, miner, camera, window, renderer);
    updateSpiders(level, miner, camera, window, renderer);
    updateMonsters(level, miner, camera, window, renderer);
}

void updateFallingObjects(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
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

                } else if(level->map[row][col] == rockTile && level->map[row+1][col] == spiderTile) { // Crashing spiders PROBLEM
                    level->map[row][col] = emptyTile;
                    level->map[row+1][col] = fallingRockTile;
                    generateDiaOnDeath(level, row+1, col, false);

                } else if(level->map[row][col] == rockTile && level->map[row+1][col] == monsterTile) { // Crashing monsters
                    level->map[row][col] = emptyTile;
                    level->map[row+1][col] = fallingRockTile;
                    generateDiaOnDeath(level, row+1, col, true);

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
                    killMiner(level, miner, camera, window, renderer, rockTile);
                    level->map[row+1][col] = rockTile;

                } else { // Just convert to stationary
                    level->map[row][col] = rockTile;

                }
                
                // Play boulderFallSOund
                if(level->map[row+1][col] != emptyTile) {
                    playBoulderFall();
                }

            }

            if(level->map[row][col] == fallingDiamondTile) { // Falling diamonds
                level->map[row][col] = diamondTile;

            }

        }

    }
}

void updateSpiders(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {

            if(level->map[row][col] == spiderTile) {

                if(level->map[row][col+1] == playerTile) { // Kill player right
                    level->map[row][col] = emptyTile;
                    killMiner(level, miner, camera, window, renderer, spiderTile);
                    level->map[row][col+1] = movingSpiderTile;

                } else if(level->map[row+1][col] == playerTile) { // Kill player bottom
                    level->map[row][col] = emptyTile;
                    killMiner(level, miner, camera, window, renderer, spiderTile); 
                    level->map[row+1][col] = movingSpiderTile;

                } else if(level->map[row][col-1] == playerTile) { // Kill player left
                    level->map[row][col] = emptyTile;
                    killMiner(level, miner, camera, window, renderer, spiderTile);
                    level->map[row][col-1] = movingSpiderTile;

                } else if(level->map[row-1][col] == playerTile) { // Kill player up
                    killMiner(level, miner, camera, window, renderer, spiderTile);
                    level->map[row][col] = emptyTile;
                    level->map[row-1][col] = movingSpiderTile;

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

void updateMonsters(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
    
    int targetRow = miner->row;
    int targetCol = miner->col;

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {

            if(level->map[row][col] == monsterTile) {
                // kill miner if possible
                if(level->map[row][col+1] == playerTile || level->map[row][col-1] == playerTile || level->map[row+1][col] == playerTile || level->map[row-1][col] == playerTile) {
                    level->map[row][col] = emptyTile;
                    level->map[miner->row][miner->col] = monsterTile;
                    killMiner(level, miner, camera, window, renderer, monsterTile);

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

void updateWater(level_t *level) {

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == waterTile) {
                if(level->map[row+1][col] == dirtTile) { // bottom
                    level->map[row+1][col] = movingWaterTile;

                }
                if(level->map[row-1][col] == dirtTile) { // top
                    level->map[row-1][col] = movingWaterTile;

                }
                if(level->map[row][col-1] == dirtTile) { // left
                    level->map[row][col-1] = movingWaterTile;

                }
                if(level->map[row][col+1] == dirtTile) { // right
                    level->map[row][col+1] = movingWaterTile;

                }

            }

        }

    }

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(level->map[row][col] == movingWaterTile) {
                level->map[row][col] = waterTile;

            }

        }

    }

}

void generateDiaOnDeath(level_t *level, int row, int col, bool isMonster) {

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
    if(isMonster == true) {
        if(level->map[row-2][col] != borderTile && level->map[row-2][col] != playerTile) { //iki yukari
           level->map[row-2][col] = diamondTile;
        }
        if(level->map[row+2][col] != borderTile && level->map[row+2][col] != playerTile) { //iki asagi
           level->map[row+2][col] = diamondTile;
        }
        if(level->map[row][col+2] != borderTile && level->map[row][col+2] != playerTile) { //iki sag
           level->map[row][col+2] = diamondTile;
        }
        if(level->map[row][col-2] != borderTile && level->map[row][col-2] != playerTile) { //iki sol
           level->map[row][col-2] = diamondTile;
        }

    }

}

void killMiner(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer, char deathCause) {
    printf("YOU DIED\n");
    miner->lives--;
    switch(deathCause) {
        case rockTile:
            level->map[miner->row][miner->col] = rockTile;
            break;
        case spiderTile:
            level->map[miner->row][miner->col] = spiderTile;
            break;
        case monsterTile:
            level->map[miner->row][miner->col] = monsterTile;
            break;
    }
    renderOnDeath(*level, camera, window, renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    miner->row = level->startMinerRow;
    miner->col = level->startMinerCol;
    level->map[miner->row][miner->col] = playerTile;
}
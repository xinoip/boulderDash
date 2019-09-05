#include "./controls.h"
#include "../render.h"

bool isValidCellToMove(level_t level, int row, int col) {

    if(level.map[row][col] == borderTile) return false;

    return true;

}

void updateMap(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
    updateFalling(level, miner, camera, window, renderer);
    updateSpiders(level, miner, camera, window, renderer);
    updateMonsters(level, miner, camera, window, renderer);
}

void updateFalling(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {

    int dontProcess[level->row][level->col];
    for(int row = 0; row<level->row; row++) {
        for(int col = 0; col<level->col; col++) {
            dontProcess[row][col] = 0;
        }
    }

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {
            if(dontProcess[row][col] != 1) {
                switch(level->map[row][col]) {
                    case rockTile:
                        switch(level->map[row+1][col]) {
                            case emptyTile:
                                level->map[row][col] = fallingRockTile;
                            break;
                            case rockTile: // Crush onto another rock
                                if(level->map[row][col-1] == emptyTile && level->map[row+1][col-1] == emptyTile) {
                                    level->map[row][col] = emptyTile;
                                    level->map[row][col-1] = fallingRockTile;
                                    dontProcess[row][col-1] = 1;

                                } else if(level->map[row][col+1] == emptyTile && level->map[row+1][col+1] == emptyTile) {
                                    level->map[row][col] = emptyTile;
                                    level->map[row][col+1] = fallingRockTile;
                                    dontProcess[row][col+1] = 1;

                                }
                            break;
                        }
                    break;
                    case fallingRockTile:
                        switch(level->map[row+1][col]) {
                            case emptyTile:
                                level->map[row][col] = emptyTile;
                                level->map[row+1][col] = fallingRockTile;
                                dontProcess[row+1][col] = 1;
                            break;
                            case playerTile:
                                level->map[row][col] = emptyTile;
                                level->map[row+1][col] = fallingRockTile;
                                dontProcess[row+1][col] = 1;
                                killMiner(level, miner, camera, window, renderer, rockTile);
                            break;
                            case spiderR:
                            case spiderB:
                            case spiderL:
                            case spiderT:
                            case spiderTile:
                                level->map[row][col] = emptyTile;
                                level->map[row+1][col] = fallingRockTile;
                                dontProcess[row+1][col] = 1;
                                generateDiaOnDeath(level, row+1, col, false);
                            break;
                            case movingMonsterTile:
                            case monsterTile:
                                level->map[row][col] = emptyTile;
                                level->map[row+1][col] = fallingRockTile;
                                dontProcess[row+1][col] = 1;
                                generateDiaOnDeath(level, row+1, col, true);
                            break;
                            default:
                                playBoulderFall();
                                level->map[row][col] = rockTile;
                            break;
                        }
                    break;
                    case diamondTile:
                        if(level->map[row+1][col] == emptyTile) {
                            level->map[row][col] = fallingDiamondTile;
                        }
                    break;
                    case fallingDiamondTile:
                        switch(level->map[row+1][col]) {
                            case emptyTile:
                                level->map[row][col] = emptyTile;
                                level->map[row+1][col] = fallingDiamondTile;
                                dontProcess[row+1][col] = 1;
                            break;
                            case playerTile:
                                level->map[row][col] = emptyTile;
                                playDiamondCollect();
                                level->diamondCount--;
                            break;
                            default:
                                level->map[row][col] = diamondTile;
                            break;
                        }
                    break;
                }
            }
        }
    }

}

void updateSpiders(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
    int dontProcess[level->row][level->col];
    for(int row = 0; row<level->row; row++) {
        for(int col = 0; col<level->col; col++) {
            dontProcess[row][col] = 0;
        }
    }

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) { 

            if(dontProcess[row][col] != 1 && (level->map[row][col] == spiderTile || level->map[row][col] == spiderR || level->map[row][col] == spiderB || level->map[row][col] == spiderL || level->map[row][col] == spiderT)) {

                if(level->map[row][col+1] == playerTile || level->map[row+1][col] == playerTile || level->map[row][col-1] == playerTile || level->map[row-1][col] == playerTile) {
                    level->map[row][col] = emptyTile;
                    level->map[miner->row][miner->col] = spiderTile;
                    dontProcess[miner->row][miner->col] = 1;
                    killMiner(level, miner, camera, window, renderer, spiderTile);

                } else {
                    if(level->map[row][col] == spiderTile) {
                        if(level->map[row][col+1] == emptyTile) { // Right
                            level->map[row][col] = spiderR;
                            dontProcess[row][col] = 1;

                        } else if(level->map[row+1][col] == emptyTile) { // Bottom
                            level->map[row][col] = spiderR;
                            dontProcess[row][col] = 1;

                        } else if(level->map[row][col-1] == emptyTile) { // Left
                            level->map[row][col] = spiderR;
                            dontProcess[row][col] = 1;

                        } else if(level->map[row-1][col] == emptyTile) { // Top
                            level->map[row][col] = spiderR;
                            dontProcess[row][col] = 1;

                        }    
                    } else {
                        switch(level->map[row][col]) {
                            case spiderR:
                                if(level->map[row][col+1] == emptyTile) { // Right
                                    level->map[row][col] = emptyTile;
                                    level->map[row][col+1] = spiderR;
                                    dontProcess[row][col+1] = 1;

                                } else {
                                    level->map[row][col] = spiderB;
                                }       
                            break;
                            case spiderB:
                                if(level->map[row+1][col] == emptyTile) { // bottom
                                    level->map[row][col] = emptyTile;
                                    level->map[row+1][col] = spiderB;
                                    dontProcess[row+1][col] = 1;

                                } else {
                                    level->map[row][col] = spiderL;
                                }
                            break;
                            case spiderL:
                                if(level->map[row][col-1] == emptyTile) { // left
                                    level->map[row][col] = emptyTile;
                                    level->map[row][col-1] = spiderL;
                                    dontProcess[row][col-1] = 1;

                                } else {
                                    level->map[row][col] = spiderT;
                                }
                            break;
                            case spiderT:
                                if(level->map[row-1][col] == emptyTile) { // top
                                    level->map[row][col] = emptyTile;
                                    level->map[row-1][col] = spiderT;
                                    dontProcess[row-1][col] = 1;

                                } else {
                                    level->map[row][col] = spiderR;
                                }
                            break;
                        }
                    }
                }

            }

        }
    }

}

void updateMonsters(level_t *level, miner_t *miner, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
    
    int targetRow = miner->row;
    int targetCol = miner->col;

    int dontProcess[level->row][level->col];
    for(int row = 0; row<level->row; row++) {
        for(int col = 0; col<level->col; col++) {
            dontProcess[row][col] = 0;
        }
    }

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {

            if(dontProcess[row][col] != 1 && (level->map[row][col] == monsterTile || level->map[row][col] == movingMonsterTile)) {
                // kill miner if possible
                if(level->map[row][col] == movingMonsterTile && level->map[row][col+1] == playerTile || level->map[row][col-1] == playerTile || level->map[row+1][col] == playerTile || level->map[row-1][col] == playerTile) {
                    level->map[row][col] = emptyTile;
                    level->map[miner->row][miner->col] = monsterTile;
                    killMiner(level, miner, camera, window, renderer, monsterTile);

                } else if(targetRow > row) { // miner on bottom
                    if(level->map[row+1][col] == emptyTile) {
                        level->map[row][col] = emptyTile;
                        level->map[row+1][col] = movingMonsterTile;
                        dontProcess[row+1][col] = 1;

                    }

                } else if(targetRow < row) { // miner on top
                    if(level->map[row-1][col] == emptyTile) {
                        level->map[row][col] = emptyTile;
                        level->map[row-1][col] = movingMonsterTile;
                        dontProcess[row-1][col] = 1;

                    }    

                } else if(targetRow == row) { // miner on same level
                    if(targetCol > col) { // miner on right
                        if(level->map[row][col+1] == emptyTile) {
                            level->map[row][col] = emptyTile;
                            level->map[row][col+1] = movingMonsterTile;
                            dontProcess[row][col+1] = 1;

                        }

                    } else if(targetCol < col) { // miner on left
                        if(level->map[row][col-1] == emptyTile) {
                            level->map[row][col] = emptyTile;
                            level->map[row][col-1] = movingMonsterTile;
                            dontProcess[row][col-1] = 1;
                            
                        }

                    } else if(targetCol == col) { // miner on same col
                        

                    }

                } else {
                    level->map[row][col] = monsterTile;
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
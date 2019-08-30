#include "./miner.h"

miner_t createMiner(int row, int col) {
    miner_t rtr;
    rtr.row = row;
    rtr.col = col;
    rtr.level = 1;
    rtr.lives = 10;
    return rtr;
}

void updateMiner(miner_t *miner, int newRow, int newCol) {
    miner->row = newRow;
    miner->col = newCol;
}

void moveMiner(miner_t *miner, level_t *level, SDL_Event event) {
    switch (event.key.keysym.sym) {
        case SDLK_UP:
            if(isValidCellToMove(*level, miner->row-1, miner->col)) {
                // level->map[miner->row][miner->col] = emptyTile;
                // level->map[miner->row-1][miner->col] = playerTile;
                // miner->row--;
                processMove(level, miner, miner->row-1, miner->col, upDir);    
            }
            
            break;
        case SDLK_DOWN:
            if(isValidCellToMove(*level, miner->row+1, miner->col)) {
                // level->map[miner->row][miner->col] = emptyTile;
                // level->map[miner->row+1][miner->col] = playerTile;
                // miner->row++;
                processMove(level, miner, miner->row+1, miner->col, downDir);    
            }
            
            break;
        case SDLK_LEFT:
            if(isValidCellToMove(*level, miner->row, miner->col-1)) {
                // level->map[miner->row][miner->col] = emptyTile;
                // level->map[miner->row][miner->col-1] = playerTile;
                // miner->col--;
                processMove(level, miner, miner->row, miner->col-1, leftDir);    
            }
            
            break;
        case SDLK_RIGHT:
            if(isValidCellToMove(*level, miner->row, miner->col+1)) {
                // level->map[miner->row][miner->col] = emptyTile;
                // level->map[miner->row][miner->col+1] = playerTile;
                // miner->col++;
                processMove(level, miner, miner->row, miner->col+1, rightDir);    
            }
            
            break;
        default:
            break;
    }
}

void processMove(level_t *level, miner_t *miner, int newRow, int newCol, moveDirection_t dir) {

    switch(level->map[newRow][newCol]) {
        case emptyTile:
            if(level->diamondCount <= 0) {
                passLevel(level);
            }
            level->map[newRow][newCol] = playerTile;   //move miner to new cell
            level->map[miner->row][miner->col] = emptyTile;    //clear old cell
            miner->row = newRow;
            miner->col = newCol;
            break;
        case dirtTile:
            playDirtRemove();
            if(level->diamondCount <= 0) {
                passLevel(level);
            } 
            level->map[newRow][newCol] = playerTile;   //move miner to new cell
            level->map[miner->row][miner->col] = emptyTile;    //clear old cell
            miner->row = newRow;
            miner->col = newCol;
            break;
        case fallingDiamondTile:
        case diamondTile:
            playDiamondCollect();
            level->map[newRow][newCol] = playerTile;   //move miner to new cell
            level->map[miner->row][miner->col] = emptyTile;    //clear old cell
            miner->row = newRow;
            miner->col = newCol;
            
            level->diamondCount--;

            if(level->diamondCount <= 0) {
                passLevel(level);
            } 
            break;
        case fallingRockTile:
        case rockTile:
            switch (dir) {
                case leftDir:
                    if(level->map[newRow][newCol-1] == emptyTile) {   //rock moveable
                        level->map[newRow][newCol-1] = rockTile;     //move rock
                        level->map[newRow][newCol] = playerTile;       //move miner to rocks old place
                        level->map[miner->row][miner->col] = emptyTile;    //clear old miner cell
                        miner->row = newRow;
                        miner->col = newCol;
                    }
                    break;
                case rightDir:
                    if(level->map[newRow][newCol+1] == emptyTile) {   //rock moveable
                        level->map[newRow][newCol+1] = rockTile;     //move rock
                        level->map[newRow][newCol] = playerTile;       //move miner to rocks old place
                        level->map[miner->row][miner->col] = emptyTile;    //clear old miner cell
                        miner->row = newRow;
                        miner->col = newCol;
                    }
                    break;
                default:
                    break;
                }
            break;
        case openDoorTile:   // DOOR LEVEL CHANGE
            miner->level++;
            switch (miner->level) {
            case 2:
                fillLevel(level, "./assets/maps/cave_2.txt");    
                break;
            case 3:
                fillLevel(level, "./assets/maps/cave_3.txt");
                break;
            case 4:
                fillLevel(level, "./assets/maps/cave_4.txt");
                break;
            case 5:
                fillLevel(level, "./assets/maps/cave_5.txt");
                break;             
            default:
                break;
            }
            updateMiner(miner, level->startMinerRow, level->startMinerCol);
            break;
        default:
            break;
    }

}

void passLevel(level_t *level) {

    for(int row = 0; row < level->row; row++) {
        for(int col = 0; col < level->col; col++) {

            if(level->map[row][col] == closedDoorTile) {
                level->map[row][col] = openDoorTile;
                return;

            }

        }
    }

}
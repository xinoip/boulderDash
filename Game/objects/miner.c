#include "./miner.h"

miner_t createMiner(int row, int col) {
    miner_t rtr;
    rtr.row = row;
    rtr.col = col;
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
                // level->map[miner->row][miner->col] = 'E';
                // level->map[miner->row-1][miner->col] = 'P';
                // miner->row--;
                processMove(level, miner, miner->row-1, miner->col, upDir);    
            }
            
            break;
        case SDLK_DOWN:
            if(isValidCellToMove(*level, miner->row+1, miner->col)) {
                // level->map[miner->row][miner->col] = 'E';
                // level->map[miner->row+1][miner->col] = 'P';
                // miner->row++;
                processMove(level, miner, miner->row+1, miner->col, downDir);    
            }
            
            break;
        case SDLK_LEFT:
            if(isValidCellToMove(*level, miner->row, miner->col-1)) {
                // level->map[miner->row][miner->col] = 'E';
                // level->map[miner->row][miner->col-1] = 'P';
                // miner->col--;
                processMove(level, miner, miner->row, miner->col-1, leftDir);    
            }
            
            break;
        case SDLK_RIGHT:
            if(isValidCellToMove(*level, miner->row, miner->col+1)) {
                // level->map[miner->row][miner->col] = 'E';
                // level->map[miner->row][miner->col+1] = 'P';
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
        case 'E':
            level->map[newRow][newCol] = 'P';   //move miner to new cell
            level->map[miner->row][miner->col] = 'E';    //clear old cell
            miner->row = newRow;
            miner->col = newCol;
            break;
        case 'D':
            level->map[newRow][newCol] = 'P';   //move miner to new cell
            level->map[miner->row][miner->col] = 'E';    //clear old cell
            miner->row = newRow;
            miner->col = newCol;
            break;
        case 'X':
            level->map[newRow][newCol] = 'P';   //move miner to new cell
            level->map[miner->row][miner->col] = 'E';    //clear old cell
            miner->row = newRow;
            miner->col = newCol;

            level->diamondCount--;

            if(level->diamondCount <= 0) {
                passLevel(level);
            }
            break;
        case 'R':
            switch (dir) {
                case leftDir:
                    if(level->map[newRow][newCol-1] == 'E') {   //rock moveable
                        level->map[newRow][newCol-1] = 'R';     //move rock
                        level->map[newRow][newCol] = 'P';       //move miner to rocks old place
                        level->map[miner->row][miner->col] = 'E';    //clear old miner cell
                        miner->row = newRow;
                        miner->col = newCol;
                    }
                    break;
                case rightDir:
                    if(level->map[newRow][newCol+1] == 'E') {   //rock moveable
                        level->map[newRow][newCol+1] = 'R';     //move rock
                        level->map[newRow][newCol] = 'P';       //move miner to rocks old place
                        level->map[miner->row][miner->col] = 'E';    //clear old miner cell
                        miner->row = newRow;
                        miner->col = newCol;
                    }
                    break;
                default:
                    break;
                }
            break;
        case 'Z':   // DOOR LEVEL CHANGE
            fillLevel(level, "./assets/maps/mapX.txt");
            updateMiner(miner, level->startMinerRow, level->startMinerCol);
            break;
        default:
            break;
    }

}

void passLevel(level_t *level) {
    
    int randomRow, randomCol;

    do {
        randomRow = rand() % level->row;
        randomCol = rand() % level->col;
        if(randomRow < level->row && randomCol < level->col) {
            if(level->map[randomRow][randomCol] == 'E') {
                break;
            }
        }
    } while(1);

    level->map[randomRow][randomCol] = 'Z';

}
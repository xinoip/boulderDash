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
                level->map[miner->row][miner->col] = 'E';
                level->map[miner->row-1][miner->col] = 'P';
                miner->row--;    
            }
            
            break;
        case SDLK_DOWN:
            if(isValidCellToMove(*level, miner->row+1, miner->col)) {
                level->map[miner->row][miner->col] = 'E';
                level->map[miner->row+1][miner->col] = 'P';
                miner->row++;    
            }
            
            break;
        case SDLK_LEFT:
            if(isValidCellToMove(*level, miner->row, miner->col-1)) {
                level->map[miner->row][miner->col] = 'E';
                level->map[miner->row][miner->col-1] = 'P';
                miner->col--;    
            }
            
            break;
        case SDLK_RIGHT:
            if(isValidCellToMove(*level, miner->row, miner->col+1)) {
                level->map[miner->row][miner->col] = 'E';
                level->map[miner->row][miner->col+1] = 'P';
                miner->col++;    
            }
            
            break;
        default:
            break;
    }
}
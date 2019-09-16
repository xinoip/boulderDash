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
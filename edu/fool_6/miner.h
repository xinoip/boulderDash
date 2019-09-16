#ifndef MINER_H
#define MINER_H

typedef struct Miner {
    int row, col;
} miner_t;

miner_t createMiner(int row, int col);

void updateMiner(miner_t *miner, int newRow, int newCol);

#endif
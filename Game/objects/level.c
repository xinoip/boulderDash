#include <stdio.h>
#include "./level.h"

//create level_t and allocate space for level array
level_t createLevel(int row, int col, int minerRow, int minerCol) {
    level_t rtr;
    rtr.row = row;
    rtr.col = col;
    rtr.startMinerRow = minerRow;
    rtr.startMinerCol = minerCol;

    rtr.map = (char **) malloc(row * sizeof(char *));

    for(int i = 0; i < row; i++) {
        rtr.map[i] = (char *) malloc(col * sizeof(char));

    }
    return rtr;
}

//create level_t from level using createLevel function, and fill it with data from filename
//probably a memory leak here, before filling destroy old map pls
void fillLevel(level_t *level, char *filename) {

    int LINE_LENGTH = 255;
    char buffer[LINE_LENGTH];
    int row, col, minerRow, minerCol;

    char currChar;

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("File io error!!");
        return;

    }

    fscanf(fp, "%d", &row);
    fscanf(fp, "%d", &col);

    //fscanf(fp, "%c", &currChar); //read carriage return only for windows files
    fscanf(fp, "%c", &currChar); //read new line

    fscanf(fp, "%d", &minerRow);
    fscanf(fp, "%d", &minerCol);

    *level = createLevel(row, col, minerRow, minerCol);

    while(currChar != '-') {
        fscanf(fp, "%c", &currChar);
    }
    //fscanf(fp, "%c", &currChar); //read carriage return only for windows files
    fscanf(fp, "%c", &currChar); //read new line
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            fscanf(fp, "%c", &currChar);
            level->map[i][j] = currChar;
        }
        //fscanf(fp, "%c", &currChar); //read carriage return only for windows files
        fscanf(fp, "%c", &currChar); //read new line
    }

}

void printLevel(level_t level) {
    for(int i = 0; i < level.row; i++) {
        for(int j = 0; j < level.col; j++) {
            //level.map[i][j] = 'A';
            printf("%c", level.map[i][j]);
        }
        printf("\n");
    }
}
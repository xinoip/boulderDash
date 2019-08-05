#include <stdio.h>
#include "./level.h"

level_t createLevel(int row, int col) {
    level_t rtr;
    rtr.row = row;
    rtr.col = col;

    rtr.map = (char **) malloc(row * sizeof(char *));

    for(int i = 0; i < row; i++) {
        rtr.map[i] = (char *) malloc(col * sizeof(char));

    }
    return rtr;
}

void fillLevel(level_t *level, char *filename) {

    int LINE_LENGTH = 255;
    char buffer[LINE_LENGTH];
    int row, col;

    char currChar;

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("File io error!!");
        return;

    }

    fscanf(fp, "%d", &row);
    fscanf(fp, "%d", &col);
    *level = createLevel(row, col);

    while(currChar != '-') {
        fscanf(fp, "%c", &currChar);
    }
    fscanf(fp, "%c", &currChar); //read carriage return only for windows files
    fscanf(fp, "%c", &currChar); //read new line
    
    for(int i = 0; i < row; i++) {
        for(int j = 0; j < col; j++) {
            fscanf(fp, "%c", &currChar);
            level->map[i][j] = currChar;
        }
        fscanf(fp, "%c", &currChar); //read carriage return only for windows files
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
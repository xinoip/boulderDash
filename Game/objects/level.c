#include <stdio.h>
#include "./level.h"

void freeOldMap(level_t *level) {
    if(level == NULL) return;
    for(int i = 0; i < level->row; i++) {
        free(level->map[i]);

    }
    free(level->map);
}

//create level_t and allocate space for level array
level_t createLevel(int row, int col, int minerRow, int minerCol, int diamondCount, char *levelName, int timeLimit, int waterMs) {
    level_t rtr;
    rtr.row = row;
    rtr.col = col;
    rtr.startMinerRow = minerRow;
    rtr.startMinerCol = minerCol;
    rtr.diamondCount = diamondCount;

    rtr.name = malloc(sizeof(char) * 255);
    strcpy(rtr.name, levelName);

    rtr.timeLimit = timeLimit;
    rtr.timeCpy = timeLimit;
    rtr.waterMs = waterMs;
    

    rtr.map = (char **) malloc(row * sizeof(char *));

    for(int i = 0; i < row; i++) {
        rtr.map[i] = (char *) malloc(col * sizeof(char));

    }
    return rtr;
}

//create level_t from level using createLevel function, and fill it with data from filename
void fillLevel(level_t *level, char *filename) {

    int LINE_LENGTH = 255;
    char buffer[LINE_LENGTH];
    int row, col, minerRow, minerCol;
    int diamondCount;
    int waterMs;
    int timeLimit;

    char currChar;

    FILE *fp = fopen(filename, "r");
    if(fp == NULL) {
        printf("File io error!!");
        return;

    }

    fgets(buffer, 255, fp);
    for(int i = 0; i < 255; i++) {
        if(buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        }
    }

    fscanf(fp, "%d", &row);
    fscanf(fp, "%d", &col);

    //fscanf(fp, "%c", &currChar); //read carriage return only for windows files
    fscanf(fp, "%c", &currChar); //read new line

    fscanf(fp, "%d", &minerRow);
    fscanf(fp, "%d", &minerCol);

    //fscanf(fp, "%c", &currChar); //read carriage return only for windows files
    fscanf(fp, "%c", &currChar); //read new line

    fscanf(fp, "%d", &diamondCount);

    //fscanf(fp, "%c", &currChar); //read carriage return only for windows files
    fscanf(fp, "%c", &currChar); //read new line

    fscanf(fp, "%d", &timeLimit);
    fscanf(fp, "%d", &waterMs);

    freeOldMap(level);
    *level = createLevel(row, col, minerRow, minerCol, diamondCount, buffer, timeLimit, waterMs);

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
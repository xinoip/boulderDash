#ifndef BASE_H
#define BASE_H

#define TILE_WIDTH 64
#define TILE_HEIGHT 64

typedef enum {false, true} bool;

#define borderTile 'W'
#define diamondTile '*'
#define dirtTile 'D'
#define openDoorTile 'z'
#define emptyTile 'E'
#define monsterTile 'M'
#define playerTile 'P'
#define rockTile 'R'
#define spiderTile 'S'
#define fallingDiamondTile 'x'
#define movingMonsterTile 'm'
#define fallingRockTile 'r'
#define movingSpiderTile 's'
#define waterTile 'G'
#define movingWaterTile 'g'
#define closedDoorTile 'Z'

#define spiderR '6'
#define spiderL '4'
#define spiderB '2'
#define spiderT '8'

#endif
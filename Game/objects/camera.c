#include "./camera.h"

//initializing the center of camera should be tweaked
camera_t createCamera(pioWindow_t window) {
    camera_t rtr;
    rtr.rowOffSet = window.width / TILE_WIDTH;
    rtr.colOffSet = window.height / TILE_HEIGHT;
    return rtr;
}

void updateCamera(camera_t *camera, pioWindow_t window) {
    camera->rowOffSet = window.width / TILE_WIDTH;
    camera->colOffSet = window.height / TILE_HEIGHT;
}

bool isInsideCamera(pioWindow_t window, camera_t camera,int row, int col) {
    int leftCol = camera.col - camera.colOffSet;
    int rightCol = camera.col + camera.colOffSet;
    int upRow = camera.row - camera.rowOffSet;
    int downRow = camera.row + camera.rowOffSet;

    return (row >= upRow && row <= downRow && col >= leftCol && col <= rightCol);  
}

void updateCameraPosition(camera_t *camera, int newRow, int newCol, int limitRow, int limitCol) {
    
    camera->row = newRow;
    camera->col = newCol;

    int leftLimit = camera->col - (camera->rowOffSet/2);
    int rightLimit = camera->col + (camera->rowOffSet/2);
    int upLimit = camera->row - (camera->colOffSet/2);
    int downLimit = camera->row + (camera->colOffSet/2);

    while(leftLimit < 0) {
        camera->col++;
        leftLimit = camera->col - (camera->rowOffSet/2);
    }

    while(rightLimit > limitCol) {
        camera->col--;
        rightLimit = camera->col + (camera->rowOffSet/2);
    }

    while(upLimit < 0) {
        camera->row++;
        upLimit = camera->row - (camera->colOffSet/2);
    }

    while(downLimit > limitRow) {
        camera->row--;
        downLimit = camera->row + (camera->colOffSet/2);
    }

}
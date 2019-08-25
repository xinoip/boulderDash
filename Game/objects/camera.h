#ifndef CAMERA_H
#define CAMERA_H

#include "../base.h"
#include "../wrappers/pio-window.h"

typedef struct Camera {
    int row, col;
    int rowOffSet, colOffSet;
} camera_t;

camera_t createCamera(pioWindow_t window);

void updateCamera(camera_t *camera, pioWindow_t window);

bool isInsideCamera(pioWindow_t window, camera_t camera,int row, int col);

void updateCameraPosition(camera_t *camera, int newRow, int newCol, int limitRow, int limitCol);

#endif
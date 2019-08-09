#include "./render.h"

pioTexture_t playerTexture;
pioTexture_t emptyTexture;
pioTexture_t dirtTexture;
pioTexture_t borderTexture;
pioTexture_t rockTexture;

bool loadMedia(SDL_Renderer *renderer) {

    bool success = true;

    playerTexture = loadPioTexture("./assets/image/playerTexture.png", renderer);
    resizePioTexture(&playerTexture, TILE_WIDTH, TILE_HEIGHT);
    if(playerTexture.texture == NULL) {
        printf("Failed to load playerTexture image!\n");
        success = false;

    }

    emptyTexture = loadPioTexture("./assets/image/emptyTexture.png", renderer);
    resizePioTexture(&emptyTexture, TILE_WIDTH, TILE_HEIGHT);
    if(emptyTexture.texture == NULL) {
        printf("Failed to load emptyTexture image!\n");
        success = false;

    }

    dirtTexture = loadPioTexture("./assets/image/dirtTexture.png", renderer);
    resizePioTexture(&dirtTexture, TILE_WIDTH, TILE_HEIGHT);
    if(dirtTexture.texture == NULL) {
        printf("Failed to load dirtTexture image!\n");
        success = false;

    }

    borderTexture = loadPioTexture("./assets/image/borderTexture.png", renderer);
    resizePioTexture(&borderTexture, TILE_WIDTH, TILE_HEIGHT);
    if(borderTexture.texture == NULL) {
        printf("Failed to load borderTexture image!\n");
        success = false;

    }

    rockTexture = loadPioTexture("./assets/image/rockTexture.png", renderer);
    resizePioTexture(&rockTexture, TILE_WIDTH, TILE_HEIGHT);
    if(rockTexture.texture == NULL) {
        printf("Failed to load rockTexture image!\n");
        success = false;

    }

    return success;

}

void closeMedia() {
    destroyPioTexture(&dirtTexture);
    destroyPioTexture(&borderTexture);
    destroyPioTexture(&playerTexture);
    destroyPioTexture(&emptyTexture);
    destroyPioTexture(&rockTexture);
}

void renderMap(level_t level, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);
    
    //i specifies the Y coordinate of render pixel
    for(int i = 0; i < level.row; i++) {
        //j specifies the X coordinate of render pixel
        for(int j = 0; j < level.col; j++) {
            tile_t currentTile = createTile(i, j);
            tile_t cam = createTile(camera.row, camera.col);
            //int topBarMenu = 30;
            int diffX = (window.width / 2) - cam.center_x; 
            int diffY = (window.height / 2) - cam.center_y ;//+ topBarMenu;
            
            if(isInsideCamera(window, camera, i, j)) {
                switch (level.map[i][j]) {
                case 'W':
                    renderPioTexture(borderTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case 'P':
                    renderPioTexture(playerTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case 'D':
                    renderPioTexture(dirtTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case 'E':
                    renderPioTexture(emptyTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case 'R':
                    renderPioTexture(rockTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case 'B':
                    renderPioTexture(rockTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                default:
                    break;
                }
            }

        }
    }
}
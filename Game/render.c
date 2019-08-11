#include "./render.h"

pioTexture_t playerTexture;
pioTexture_t emptyTexture;
pioTexture_t dirtTexture;
pioTexture_t borderTexture;
pioTexture_t rockTexture;
pioTexture_t diamondTexture;
pioTexture_t doorTexture;

TTF_Font *gFont = NULL;
pioTextFont_t mainText;
pioTextFont_t diamondCount;

bool loadMedia(SDL_Renderer *renderer) {

    bool success = true;

    gFont = TTF_OpenFont("./assets/font/zig.ttf", 28);
    if(gFont == NULL) {
        printf("Failed to load zig font! SDL_ttf Error: %s\n",TTF_GetError());
        success = false;

    }
    SDL_Color textColor = {255,255,255};
    mainText = loadPioTextFont("init", textColor, gFont, renderer);
    resizePioTexture(&(mainText.texture), 32, 32);
    if(mainText.texture.texture == NULL) {
        printf("Failed to load mainText text image!\n");
        success = false;
        
    }
    diamondCount = loadPioTextFont("init", textColor, gFont, renderer);
    resizePioTexture(&(diamondCount.texture), 32, 32);
    if(diamondCount.texture.texture == NULL) {
        printf("Failed to load diamondCount text image!\n");
        success = false;
        
    }
    

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

    diamondTexture = loadPioTexture("./assets/image/diamondTexture.png", renderer);
    resizePioTexture(&diamondTexture, TILE_WIDTH, TILE_HEIGHT);
    if(diamondTexture.texture == NULL) {
        printf("Failed to load diamondTexture image!\n");
        success = false;

    }

    doorTexture = loadPioTexture("./assets/image/doorTexture.png", renderer);
    resizePioTexture(&doorTexture, TILE_WIDTH, TILE_HEIGHT);
    if(doorTexture.texture == NULL) {
        printf("Failed to load doorTexture image!\n");
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
                case 'X':
                    renderPioTexture(diamondTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case 'Z':
                    renderPioTexture(doorTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                default:
                    break;
                }
            }

        }
    }

    renderGameBar(level, window, renderer);

}

void renderGameBar(level_t level, pioWindow_t window, SDL_Renderer *renderer) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 30);
    
    SDL_Rect gameBar;
    gameBar.x = 0,
    gameBar.y = 0,
    gameBar.w = window.width,
    gameBar.h = 50;

    SDL_RenderFillRect(renderer, &gameBar);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

    updatePioTextFont(&mainText, "Main", renderer);
    //updatePioTextFont(&diamondCount, "30D", renderer);
    
    renderPioTextureCornered(mainText.texture, 0, 0, renderer);
    renderPioTextureCornered(diamondCount.texture, 320, 0, renderer);

}

void updateGameBar(level_t level, SDL_Renderer *renderer) {
    updatePioTextFont(&diamondCount, "69D", renderer);
    //renderPioTextureCornered(diamondCount.texture, 320, 0, renderer);
}
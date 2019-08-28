#include "./render.h"

pioTexture_t playerTexture;
pioTexture_t emptyTexture;
pioTexture_t dirtTexture;
pioTexture_t borderTexture;
pioTexture_t rockTexture;
pioTexture_t diamondTexture;
pioTexture_t doorTexture;
pioTexture_t spiderTexture;
pioTexture_t monsterTexture;
pioTexture_t waterTexture;
pioTexture_t pauseTexture;
pioTexture_t playTexture;
pioTexture_t livesTexture;
pioTexture_t timeTexture;
pioTexture_t diamondSymbolTexture;

TTF_Font *gFont = NULL;
pioTextFont_t mainText;
pioTextFont_t diamondCount;
pioTextFont_t mapTimer;
pioTextFont_t minerLives;
pioTextFont_t levelName;

Mix_Music *gMusic = NULL;
Mix_Chunk *boulderFall = NULL;
Mix_Chunk *diamondCollect = NULL;
Mix_Chunk *dirtRemove = NULL;

bool loadMedia(SDL_Renderer *renderer) {

    bool success = true;

    //Load music
    gMusic = Mix_LoadMUS("./assets/sounds/music.ogg");
    if(gMusic == NULL) {
        printf( "Failed to load sound gMusic! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Load sounds
    boulderFall = Mix_LoadWAV("./assets/sounds/boulderFall.ogg");
    if(boulderFall == NULL) {
        printf( "Failed to load sound boulderFall! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Load sounds
    diamondCollect = Mix_LoadWAV("./assets/sounds/diamondCollect.ogg");
    if(diamondCollect == NULL) {
        printf( "Failed to load sound diamondCollect! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    //Load sounds
    dirtRemove = Mix_LoadWAV("./assets/sounds/dirtRemove.ogg");
    if(dirtRemove == NULL) {
        printf( "Failed to load sound dirtRemove! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    gFont = TTF_OpenFont("./assets/font/zig.ttf", 28);
    if(gFont == NULL) {
        printf("Failed to load zig font! SDL_ttf Error: %s\n",TTF_GetError());
        success = false;

    }
    SDL_Color textColor = {255,255,255};
    levelName = loadPioTextFont("init", textColor, gFont, renderer);
    resizePioTexture(&(levelName.texture), 32, 32);
    if(levelName.texture.texture == NULL) {
        printf("Failed to load levelName text image!\n");
        success = false;
        
    }
    diamondCount = loadPioTextFont("init", textColor, gFont, renderer);
    resizePioTexture(&(diamondCount.texture), 32, 32);
    if(diamondCount.texture.texture == NULL) {
        printf("Failed to load diamondCount text image!\n");
        success = false;
        
    }
    mapTimer = loadPioTextFont("init", textColor, gFont, renderer);
    resizePioTexture(&(mapTimer.texture), 32, 32);
    if(mapTimer.texture.texture == NULL) {
        printf("Failed to load mapTimer text image!\n");
        success = false;
        
    }
    minerLives = loadPioTextFont("init", textColor, gFont, renderer);
    resizePioTexture(&(minerLives.texture), 32, 32);
    if(minerLives.texture.texture == NULL) {
        printf("Failed to load minerLives text image!\n");
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

    spiderTexture = loadPioTexture("./assets/image/spiderTexture.png", renderer);
    resizePioTexture(&spiderTexture, TILE_WIDTH, TILE_HEIGHT);
    if(spiderTexture.texture == NULL) {
        printf("Failed to load spiderTexture image!\n");
        success = false;

    }

    monsterTexture = loadPioTexture("./assets/image/monsterTexture.png", renderer);
    resizePioTexture(&monsterTexture, TILE_WIDTH, TILE_HEIGHT);
    if(monsterTexture.texture == NULL) {
        printf("Failed to load monsterTexture image!\n");
        success = false;

    }

    waterTexture = loadPioTexture("./assets/image/waterTexture.jpg", renderer);
    resizePioTexture(&waterTexture, TILE_WIDTH, TILE_HEIGHT);
    if(waterTexture.texture == NULL) {
        printf("Failed to load waterTexture image!\n");
        success = false;

    }

    pauseTexture = loadPioTexture("./assets/image/pauseTexture.png", renderer);
    resizePioTexture(&pauseTexture, 32, 32);
    if(pauseTexture.texture == NULL) {
        printf("Failed to load pauseTexture image!\n");
        success = false;

    }

    playTexture = loadPioTexture("./assets/image/playTexture.png", renderer);
    resizePioTexture(&playTexture, 32, 32);
    if(playTexture.texture == NULL) {
        printf("Failed to load playTexture image!\n");
        success = false;

    }

    livesTexture = loadPioTexture("./assets/image/livesTexture.png", renderer);
    resizePioTexture(&livesTexture, 32, 32);
    if(livesTexture.texture == NULL) {
        printf("Failed to load livesTexture image!\n");
        success = false;

    }

    timeTexture = loadPioTexture("./assets/image/timeTexture.png", renderer);
    resizePioTexture(&timeTexture, 32, 32);
    if(timeTexture.texture == NULL) {
        printf("Failed to load timeTexture image!\n");
        success = false;

    }

    diamondSymbolTexture = loadPioTexture("./assets/image/diamondTexture.png", renderer);
    resizePioTexture(&diamondSymbolTexture, 32, 32);
    if(diamondSymbolTexture.texture == NULL) {
        printf("Failed to load diamondSymbolTexture image!\n");
        success = false;

    }

    return success;

}

void startMusic() {
    Mix_PlayMusic(gMusic, 0);
}

void playBoulderFall() {
    Mix_PlayChannel(-1, boulderFall, 0);
}

void playDiamondCollect() {
    Mix_PlayChannel(-1, diamondCollect, 0);
}

void playDirtRemove() {
    Mix_PlayChannel(-1, dirtRemove, 0);
}

void closeMedia() {
    destroyPioTexture(&dirtTexture);
    destroyPioTexture(&borderTexture);
    destroyPioTexture(&playerTexture);
    destroyPioTexture(&emptyTexture);
    destroyPioTexture(&rockTexture);
    destroyPioTexture(&diamondTexture);
    destroyPioTexture(&doorTexture);
    destroyPioTexture(&spiderTexture);
    destroyPioTexture(&monsterTexture);
    destroyPioTexture(&waterTexture);
    destroyPioTexture(&pauseTexture);
    destroyPioTexture(&livesTexture);
    destroyPioTexture(&timeTexture);
}

void renderMap(level_t level, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
    SDL_RenderClear(renderer);
    //i specifies the Y coordinate of render pixel
    for(int i = 0; i < level.row; i++) {
        //j specifies the X coordinate of render pixel
        for(int j = 0; j < level.col; j++) {
            tile_t currentTile = createTile(i, j);
            tile_t cam = createTile(camera.row, camera.col);
            int topBarMenu = 32;
            // int diffX = (window.width / 2) - cam.center_x; 
            // int diffY = (window.height / 2) - cam.center_y + topBarMenu;
            int diffX = (window.width/2) - cam.center_x;
            int diffY = (window.height/2) - cam.center_y;

            if(isInsideCamera(window, camera, i, j)) {
                switch (level.map[i][j]) {
                case borderTile:
                    renderPioTexture(borderTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case playerTile:
                    renderPioTexture(playerTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case dirtTile:
                    renderPioTexture(dirtTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case emptyTile:
                    renderPioTexture(emptyTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case rockTile:
                    renderPioTexture(rockTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                //case 'B':
                case fallingRockTile:
                    renderPioTexture(rockTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                //case 'X':
                case diamondTile:
                    renderPioTexture(diamondTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case fallingDiamondTile:
                    renderPioTexture(diamondTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                //case 'Z':
                case doorTile:
                    renderPioTexture(doorTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case spiderTile:
                    renderPioTexture(spiderTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case movingSpiderTile:
                    renderPioTexture(spiderTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                //case 'L':
                case monsterTile:
                    renderPioTexture(monsterTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case movingMonsterTile:
                    renderPioTexture(monsterTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                case waterTile:
                    renderPioTexture(waterTexture, currentTile.center_x + diffX, currentTile.center_y + diffY, renderer);
                    break;
                default:
                    break;
                }
            }

        }
    }

}

void renderGameBar(level_t level, pioWindow_t window, SDL_Renderer *renderer, bool isPaused) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 30);
    
    SDL_Rect gameBar;
    gameBar.x = 0,
    gameBar.y = 0,
    gameBar.w = window.width,
    gameBar.h = 32;

    SDL_RenderFillRect(renderer, &gameBar);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

    //updatePioTextFont(&mainText, "Main", renderer);
    //updatePioTextFont(&diamondCount, "0D", renderer);
    
    renderPioTextureCornered(levelName.texture, 0, 0, renderer);
    //renderPioTextureCornered(diamondCount.texture, 320, 0, renderer);

    if(isPaused) {
        renderPioTextureCornered(pauseTexture, window.width - 32, 0, renderer);
    } else {
        renderPioTextureCornered(playTexture, window.width - 32, 0, renderer);
    }
    
    renderPioTextureCornered(livesTexture, window.width - 64, 0, renderer);
    renderPioTextureCornered(minerLives.texture, window.width - 128, 0, renderer);

    renderPioTextureCornered(timeTexture, (window.width / 2) + 32, 0, renderer);
    renderPioTextureCornered(mapTimer.texture, (window.width / 2) + 64, 0, renderer);

    renderPioTextureCornered(diamondCount.texture, (window.width / 2) - 32, 0, renderer);
    renderPioTextureCornered(diamondSymbolTexture, (window.width / 2) - 64, 0, renderer);

}

void updateGameBar(level_t level, SDL_Renderer *renderer, int lives) {
    char diaText[5];
    char mapTimeText[12];
    char minerLivesText[12];
    sprintf(minerLivesText, "%d", lives);
    sprintf(mapTimeText, "%d", level.timeLimit);
    if(level.diamondCount <= 0) {
        updatePioTextFont(&diamondCount, "Door Open", renderer);    
    } else {
        sprintf(diaText, "%d", level.diamondCount);
        updatePioTextFont(&diamondCount, diaText, renderer);
    }
    updatePioTextFont(&mapTimer, mapTimeText, renderer);
    updatePioTextFont(&minerLives, minerLivesText, renderer);
    updatePioTextFont(&levelName, level.name, renderer);
    //renderPioTextureCornered(diamondCount.texture, 320, 0, renderer);
}

void renderOnDeath(level_t level, camera_t camera, pioWindow_t window, SDL_Renderer *renderer) {
    renderMap(level, camera, window, renderer);
    renderGameBar(level, window, renderer, true);
}
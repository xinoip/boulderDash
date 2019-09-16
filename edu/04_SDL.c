#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#define true 1
#define false 0
#define bool _Bool

typedef enum KeyPressSurfaces {
	sPRESS_DEFAULT,
	sPRESS_UP,
	sPRESS_DOWN,
	sPRESS_LEFT,
	sPRESS_RIGHT,
	sPRESS_TOTAL
} keyPressSurfaces_t;

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gXOut = NULL;

SDL_Surface *sPressSurfaces[sPRESS_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

bool init() {

	bool success = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		success = false;

	} else {
		gWindow = SDL_CreateWindow("Piosdl", SDL_WINDOWPOS_UNDEFINED
			, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT
			, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			success = false;

		} else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);

		}

	}

	return success;

}

bool loadMedia() {

	bool success = true;

	sPressSurfaces[sPRESS_DEFAULT] = SDL_LoadBMP("./src-imgs/press.bmp");
	if(sPressSurfaces[sPRESS_DEFAULT] == NULL) {
		printf("Unable to load image press! SDL_Error\n", SDL_GetError());
		success = false;

	}

	sPressSurfaces[sPRESS_UP] = SDL_LoadBMP("./src-imgs/up.bmp");
	if(sPressSurfaces[sPRESS_UP] == NULL) {
		printf("Unable to load image up! SDL_Error\n", SDL_GetError());
		success = false;

	}

	sPressSurfaces[sPRESS_DOWN] = SDL_LoadBMP("./src-imgs/down.bmp");
	if(sPressSurfaces[sPRESS_DOWN] == NULL) {
		printf("Unable to load image down! SDL_Error\n", SDL_GetError());
		success = false;

	}

	sPressSurfaces[sPRESS_LEFT] = SDL_LoadBMP("./src-imgs/left.bmp");
	if(sPressSurfaces[sPRESS_LEFT] == NULL) {
		printf("Unable to load image left! SDL_Error\n", SDL_GetError());
		success = false;

	}

	sPressSurfaces[sPRESS_RIGHT] = SDL_LoadBMP("./src-imgs/right.bmp");
	if(sPressSurfaces[sPRESS_RIGHT] == NULL) {
		printf("Unable to load image right! SDL_Error\n", SDL_GetError());
		success = false;

	}

	gXOut = SDL_LoadBMP("./src-imgs/x.bmp");
	if(gXOut == NULL) {
		printf("Unable to load image %s! SDL_Error: %s\n", "02_myImage.bmp", SDL_GetError());
		success = false;

	}

	return success;

}

void closeAll() {

	for(int i = 0; i < sPRESS_TOTAL; i++) {
		SDL_FreeSurface(sPressSurfaces[i]);
		sPressSurfaces[i] = NULL;

	}

	SDL_FreeSurface(gXOut);
	gXOut = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();

}

int main(int argc, char *args[]) {

	if(!init()) {
		printf("Failed to init!\n");
 
	} else {
		if(!loadMedia()) {
			printf("Failed to loadMedia!\n");

		} else {
			bool quit = false;

			SDL_Event e;

			gCurrentSurface = sPressSurfaces[sPRESS_DEFAULT];

			while(!quit) {
				while(SDL_PollEvent(&e) != 0) {
					if(e.type == SDL_QUIT) {
						quit = true;

					} else if(e.type == SDL_KEYDOWN) {
						switch(e.key.keysym.sym) {
							case SDLK_UP:
							printf("up\n");
							gCurrentSurface = sPressSurfaces[sPRESS_UP];
							break;
							case SDLK_DOWN:
							printf("down\n");
							gCurrentSurface = sPressSurfaces[sPRESS_DOWN];
							break;
							case SDLK_LEFT:
							printf("left\n");
							gCurrentSurface = sPressSurfaces[sPRESS_LEFT];
							break;
							case SDLK_RIGHT:
							printf("right\n");
							gCurrentSurface = sPressSurfaces[sPRESS_RIGHT];
							break;
							default:
							printf("other\n");
							gCurrentSurface = sPressSurfaces[sPRESS_DEFAULT];
							break;

						}

					}

				}

				SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);

				SDL_UpdateWindowSurface(gWindow);

			}

		}

	}

	closeAll();

}
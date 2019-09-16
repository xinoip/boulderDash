#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#define true 1
#define false 0
#define bool _Bool

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gXOut = NULL;

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

	gXOut = SDL_LoadBMP("./src-imgs/x.bmp");
	if(gXOut == NULL) {
		printf("Unable to load image %s! SDL_Error: %s\n", "02_myImage.bmp", SDL_GetError());
		success = false;

	}

	return success;

}

void closeAll() {

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

			while(!quit) {
				while(SDL_PollEvent(&e) != 0) {
					if(e.type == SDL_QUIT) {
						quit = true;

					}

				}

				SDL_BlitSurface(gXOut, NULL, gScreenSurface, NULL);

				SDL_UpdateWindowSurface(gWindow);

			}

		}

	}

	closeAll();

}
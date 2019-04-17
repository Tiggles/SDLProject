#ifdef __APPLE__
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#elif _WIN64
#include <SDL.h>
#endif
#include <stdio.h>
#include <string>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();
SDL_Surface *loadSurface(std::string path);

enum KeyPressSurfaces
{
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface *gCurrentSurface = NULL;

SDL_Surface *loadSurface(std::string path) {
	SDL_Surface *optimizedSurface = NULL;
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		// Convert to surface format, meaning it is done once, instead every time it is rendered
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, NULL);
		if (optimizedSurface == NULL)
		{
			printf("Could not optimize %s. ERROR: %s", path.c_str(), SDL_GetError());
		}

		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not be initialized. Error: %s\n", SDL_GetError());
		return false;
	} else {
		gWindow = SDL_CreateWindow("Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be initialized. Error: %s\n", SDL_GetError());
			return false;
		}
		int imgFlags = IMG_INIT_PNG;
		// Pass image initialization flags to allow for ›
		if ( !(IMG_Init(imgFlags) & imgFlags)) {
			printf("SDL_IMAGE could not initialize! Error: %s", IMG_GetError());
			return false;
			
		}
		gScreenSurface = SDL_GetWindowSurface(gWindow);
	}
	return true;
}

bool loadMedia() {
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("Image.png");
	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("up.png");
	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("left.png");
	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("right.png");
	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("down.png");
	return true;
}

void close() {

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	SDL_Quit();
}

int main(int argc, char *args[]) {

	bool quit = false;
	printf("%i", gCurrentSurface == NULL);
	SDL_Event e;

	if (!init())
	{
		printf("Init fail\n");
		return 1;
	}

	if (!loadMedia())
	{
		printf("Load Media fail");
		return 1;
	}

	gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			else if (e.type == SDL_KEYDOWN)
			{
				switch (e.key.keysym.sym)
				{
				case SDLK_UP:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
					break;
				case SDLK_DOWN:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
					break;
				case SDLK_LEFT:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
					break;
				case SDLK_RIGHT:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
					break;
				default:
					gCurrentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
					break;
				}
			}
		}

		SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
		SDL_UpdateWindowSurface(gWindow);
	}
	close();
	return 0;
}

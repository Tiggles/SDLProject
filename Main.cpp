#pragma once

#include <stdio.h>
#include <string>
#include "SDL_Includes.hpp"
#include "Texture.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool load_media();
void close();

// Load invidivual image as texture
SDL_Texture *load_texture(std::string path);


enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
SDL_Surface* currentSurface = NULL;
SDL_Renderer* renderer = NULL;

SDL_Rect spriteClips[4];
LTexture spriteSheetTexture;

SDL_Texture *load_texture(std::string path) {
	// Final texture
	SDL_Texture *newTexture;
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
		return NULL;
	}

	newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == NULL) {
		printf("Error creating texture from %s. Error: %s\n", path.c_str(), SDL_GetError());
	}
	SDL_FreeSurface(loadedSurface);
	return newTexture;
}

SDL_Surface *loadSurface(std::string path) {
	SDL_Surface *optimizedSurface = NULL;
	SDL_Surface *loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
		return optimizedSurface;
	}
	// Convert to surface format, meaning it is done once, instead every time it is rendered
	optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, NULL);
	if (optimizedSurface == NULL) {
		printf("Could not optimize %s. ERROR: %s\n", path.c_str(), SDL_GetError());
	}

	SDL_FreeSurface(loadedSurface);

	return optimizedSurface;
}

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not be initialized. Error: %s\n", SDL_GetError());
		return false;
	} 

	window = SDL_CreateWindow("Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		printf("Window could not be initialized. Error: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		printf("Renderer could not be created. Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_SetRenderDrawColor(renderer, 0x0F, 0xFF, 0xFF, 0xFF);

	int imgFlags = IMG_INIT_PNG;
	// Pass image initialization flags to allow for ›
	if ( !(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_IMAGE could not initialize! Error: %s\n", IMG_GetError());
		return false;
	}
	screenSurface = SDL_GetWindowSurface(window);
	return true;
}

bool load_media() {
	if (!spriteSheetTexture.load_from_file("circles.png", renderer)) {
		printf("Failed to load sprite sheet texture!\n");
		return false;
	}


	//Set top left sprite
	spriteClips[0].x = 0;
	spriteClips[0].y = 0;
	spriteClips[0].w = 32;
	spriteClips[0].h = 32;

	//Set top right sprite
	spriteClips[1].x = 32;
	spriteClips[1].y = 0;
	spriteClips[1].w = 32;
	spriteClips[1].h = 32;

	//Set bottom left sprite
	spriteClips[2].x = 0;
	spriteClips[2].y = 32;
	spriteClips[2].w = 32;
	spriteClips[2].h = 32;

	//Set bottom right sprite
	spriteClips[3].x = 32;
	spriteClips[3].y = 32;
	spriteClips[3].w = 32;
	spriteClips[3].h = 32;

	return true;
}

void close() {
	spriteSheetTexture.free();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char *args[]) {
	bool quit = false;
	SDL_Event e;

	if (!init()) {
		printf("Init fail\n");
		return 1;
	}

	if (!load_media()) {
		printf("Load Media fail");
		return 1;
	}

	currentSurface = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) quit = true;
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);

		spriteSheetTexture.render(0, 0, renderer, &spriteClips[0]);

		spriteSheetTexture.render(SCREEN_WIDTH - spriteClips[1].w, 0, renderer, &spriteClips[1]);

		spriteSheetTexture.render(0, SCREEN_HEIGHT - spriteClips[1].h, renderer, &spriteClips[2]);

		spriteSheetTexture.render(SCREEN_WIDTH - spriteClips[3].w, SCREEN_HEIGHT - spriteClips[3].h, renderer, &spriteClips[3]);

		SDL_RenderPresent(renderer);
	}
	close();
	return 0;
}
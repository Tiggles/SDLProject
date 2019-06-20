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
SDL_Texture* load_texture(std::string path);


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
LTexture modulated_texture;
LTexture background_texture;

SDL_Texture* load_texture(std::string path) {
	// Final texture
	SDL_Texture* newTexture;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
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

SDL_Surface* loadSurface(std::string path) {
	SDL_Surface* optimizedSurface = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
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
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		printf("SDL_IMAGE could not initialize! Error: %s\n", IMG_GetError());
		return false;
	}
	screenSurface = SDL_GetWindowSurface(window);
	return true;
}

bool load_media() {
	if (!modulated_texture.load_from_file("squares.png", renderer)) {
		printf("Failed to load sprite sheet texture!\n");

	}
	else {
		modulated_texture.setBlendMode(SDL_BLENDMODE_BLEND);
	}

	if (!background_texture.load_from_file("circles.png", renderer)) {
		printf("Failed to load background\n");
		return false;
	}

	return true;
}

void close() {
	modulated_texture.free();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = NULL;
	window = NULL;
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[]) {
	bool quit = false;
	SDL_Event e;

	SDL_Texture* tex;

	Uint8 a = 255;

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
			else if (e.type == SDL_KEYDOWN) {
				//Increase alpha on w
				if (e.key.keysym.sym == SDLK_w)
				{
					//Cap if over 255
					if (a + 32 > 255)
					{
						a = 255;
					}
					//Increment otherwise
					else
					{
						a += 32;
					}
				}
				//Decrease alpha on s
				else if (e.key.keysym.sym == SDLK_s)
				{
					//Cap if below 0
					if (a - 32 < 0)
					{
						a = 0;
					}
					//Decrement otherwise
					else
					{
						a -= 32;
					}
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		background_texture.render(0, 0, renderer);
		modulated_texture.setAlpha(a);
		modulated_texture.render(0, 0, renderer);

		SDL_RenderPresent(renderer);
	}
	close();
	return 0;
}
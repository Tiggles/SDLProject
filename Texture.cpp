#include "Texture.hpp"
#include "SDL_Includes.hpp"
#include <stdio.h>
#include <iostream>
#include <string>

LTexture::LTexture() {
	texture = NULL;
	width = 0;
	height = 0;
}

LTexture::~LTexture() {
	free();
}

bool LTexture::load_from_file(std::string path, SDL_Renderer *renderer) {
	free();

	SDL_Texture* new_texture = nullptr;

	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	if (!loaded_surface) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
		return false;
	}

	SDL_SetColorKey(loaded_surface, SDL_TRUE, SDL_MapRGB(loaded_surface->format, 0, 0xFF, 0xFF));
	new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	if (!new_texture) {
		printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		return false;
	}
	width = loaded_surface->w;
	height = loaded_surface->h;

	texture = new_texture;
	return texture != NULL;
}

void LTexture::free() {
	if (!texture) return;
	SDL_DestroyTexture(texture);
	texture = NULL;
	width = 0;
	height = 0;
}

void LTexture::render(int x, int y, SDL_Renderer *renderer, SDL_Rect *clip) {
	SDL_Rect renderQuad = { x, y, width, height };

	if (clip != NULL) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	SDL_RenderCopy(renderer, texture, clip, &renderQuad);
}

int LTexture::get_height() {
	return height;
}

int LTexture::get_width() {
	return width;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
	SDL_SetTextureColorMod(texture, red, green, blue);
}
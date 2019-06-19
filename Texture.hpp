#pragma once
#include "SDL_Includes.hpp"
#include <string>
class LTexture {
	SDL_Texture* texture;

	int width;
	int height;

public:
	LTexture();
	~LTexture();
	bool load_from_file(std::string path, SDL_Renderer *renderer);

	void free();
	void render(int x, int y, SDL_Renderer *renderer, SDL_Rect *clip = NULL);
	int get_width();
	int get_height();

	void setColor(Uint8 red, Uint8 green, Uint8 blue);
};
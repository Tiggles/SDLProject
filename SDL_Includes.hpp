#pragma once
#ifdef __APPLE__
#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#elif _WIN64 || _WIN32
#include <SDL.h>
#include <SDL_image.h>
#endif
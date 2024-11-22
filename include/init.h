#ifndef INIT_H
#define INIT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "constants.h"

bool init_sdl(SDL_Window **window, SDL_Renderer **renderer, Mix_Music **background_music);

#endif // INIT_H


#ifndef CLEANUP_H
#define CLEANUP_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer, Mix_Music *background_music);

#endif // CLEANUP_H
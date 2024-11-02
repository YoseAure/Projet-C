#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

extern bool music_enabled;
extern bool sound_effects_enabled;

void settings(SDL_Renderer *renderer);

#endif // SETTINGS_H
#ifndef SPRITE_H
#define SPRITE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "constants.h"
#include <stdio.h>

SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer);
void getSpriteClips(SDL_Rect clips[SPRITE_ROWS][SPRITE_COLS], int rows, int cols);

#endif // SPRITE_H
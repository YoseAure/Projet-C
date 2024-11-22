#ifndef MAPS_H
#define MAPS_H

#include "constants.h"
#include <SDL2/SDL.h>

BlockType** load_map(const char *filename);
void free_map(BlockType **map, int height);
void render_map(SDL_Renderer *renderer, BlockType **map, int width, int height);

#endif // MAPS_H
#ifndef MAPS_H
#define MAPS_H

#include <SDL2/SDL.h>
#include "constants.h"
#include "game.h"

BlockType** load_map(const char *filename);
void free_map(BlockType **map, int height);
void render_map(SDL_Renderer *renderer, BlockType **map, int width, int height, int cameraX);
void reset_map(BlockType ***map);

#endif // MAPS_H
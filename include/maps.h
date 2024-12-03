#ifndef MAPS_H
#define MAPS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "constants.h"
#include "structs.h"
#include "sprite.h"

Map* load_map(const char *filename);
Map* load_map_from_csv(const char *filename);
void free_map(Map *map);
void render_map(SDL_Renderer *renderer, Map *map, int cameraX);
void reset_map(Map **map);
bool load_block_textures(SDL_Renderer *renderer);
void free_block_textures();

#endif // MAPS_H
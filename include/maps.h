#ifndef MAPS_H
#define MAPS_H

#include <SDL2/SDL.h>
#include "constants.h"
#include "game.h"

typedef struct {
    BlockType **blocks;
    int width;
    int height;
} Map;

Map* load_map(const char *filename);
void free_map(Map *map);
void render_map(SDL_Renderer *renderer, Map *map, int cameraX);
void reset_map(Map **map);

#endif // MAPS_H
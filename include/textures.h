#ifndef TEXTURES_H
#define TEXTURES_H

#include <SDL2/SDL.h>
#include "structs.h"

SDL_Texture *ground_texture = NULL;
SDL_Texture *brick_texture = NULL;
SDL_Texture *pilar_texture = NULL;
SDL_Texture *torch_texture = NULL;
SDL_Texture *coin_texture = NULL;
SDL_Texture *enemy_texture = NULL;
SDL_Texture *player_texture = NULL;
SDL_Texture *background_texture = NULL;
SDL_Texture *dragon_texture = NULL;
SDL_Texture *princess_texture = NULL;
SDL_Texture *redFlag_texture = NULL;
SDL_Texture *tileset_texture = NULL;
SDL_Rect *tile_clips = NULL;
SDL_Texture *ground_tileset_texture = NULL;
SDL_Rect *ground_tile_clips = NULL;
SDL_Texture *socks_texture = NULL;
SDL_Texture *store_texture = NULL;
SDL_Texture *surfshop_texture = NULL;
SDL_Texture *planche_surfshop_texture = NULL;
SDL_Texture *win_background = NULL;
SDL_Texture *pesty_texture = NULL;

TextureInfo texture_info[] = {
    // {{x, y, w (pixel), h (pixel)}, w (largeur de la texture), h (hauteur de la texture)}
    {{7, 4, 51, 91}, 32, 32},      // 0 GROUND
    {{244, 10, 138, 192}, 32, 32}, // 1 HOUSE
    {{9, 240, 36, 31}, 32, 32},    // 2 CHEST
    {{4, 199, 30, 25}, 32, 32},    // 3 ROCK in water
    {{175, 185, 29, 31}},          // 4 PANEL
    {{383, 64, 35, 35}, 32, 32},   // 5 WATER
    {{34, 225, 29, 27}, 32, 32},   // 6 GRASS
    {{229, 129, 23, 30}, 32, 32},  // 7 GROUND 2
    {{101, 129, 23, 27}, 32, 32},  // 8 EARTH
    {{11, 293, 37, 43}, 32, 32},   // 9 PLANCHER
    {{136, 314, 57, 35}, 32, 32},  // 10 PONTON
};

#endif // TEXTURES_H
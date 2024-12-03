#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>
#include "constants.h"

typedef struct {
    int x, y, width, height;
    BlockType type;
    bool isSolid;
} Block;

typedef struct {
    Block **blocks;
    int width;
    int height;
} Map;

typedef struct {
    int x, y, width, height;
    bool isJumping;
    int y_speed;
    Uint32 lastJump_t;
    int animation_row;
    SDL_Texture *texture;
    SDL_Rect clips[MAX_SPRITE_ROWS][MAX_SPRITE_COLS];
    int current_frame;
} Player;


typedef struct {
    int x, y, width, height;
    int x_speed, y_speed;
    int initial_x, initial_y;
    MobType type;
    int animation_row;
    SDL_Texture *texture;
    SDL_Rect clips[MAX_SPRITE_ROWS][MAX_SPRITE_COLS];
    int current_frame;
    int direction;
} Mob;

#endif // STRUCTS_H
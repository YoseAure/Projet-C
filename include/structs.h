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
    int life_points;
    int total_life_points;
    int coins_count;
    bool isJumping;
    int y_speed;
    Uint32 lastJump_t;
    Uint32 lastHit_t;
    int animation_row;
    SDL_Texture *texture;
    SDL_Rect clips[MAX_SPRITE_ROWS][MAX_SPRITE_COLS];
    int current_frame;
} Player;

typedef struct {
    SDL_Rect clip;
    int width;
    int height;
} TextureInfo;

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

typedef struct {
    char name[50];
    SDL_Texture *texture;
} Item;

typedef struct {
    Item items[MAX_ITEMS];
    int item_count;
} Inventory;

#endif // STRUCTS_H
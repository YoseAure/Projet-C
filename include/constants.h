#ifndef CONSTANTS_H
#define CONSTANTS_H

// general
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900

#define TILE_SIZE 30

// Maps
#define WORLD_WIDTH 13500
#define WORLD_HEIGHT 900

typedef enum {
    INVISIBLE,
    EMPTY,
    GROUND,
    BRICK,
    PILAR,
    TORCH,
    RED_FLAG,
    COIN,
    ENEMY,
    BLACK,
    BLOCK_DRAGON,
    BLOCK_PRINCESS,
    PLAYER
} BlockType;

typedef enum {
    DRAGON,
    PRINCESS
} MobType;

// Player
#define GRAVITY 1
#define JUMP_FORCE -20
#define JUMP_DELAY 500 // ms
#define MOVEMENT_SPEED 10
#define SCROLL_SPEED 10

#define PLAYER_SPRITE_FRAME_WIDTH 64
#define PLAYER_SPRITE_FRAME_HEIGHT 64
#define PLAYER_SPRITE_ROWS 46
#define PLAYER_SPRITE_COLS 13

#define MAX_SPRITE_ROWS 50
#define MAX_SPRITE_COLS 50

// Sprite rows for mouvements
#define JUMP 29
#define IDLE 24
#define IDLE_UP 22
#define IDLE_LEFT 23
#define IDLE_RIGHT 25
#define LEFT 35
#define RIGHT 37
#define UP 34
#define DOWN 36

// mobs
#define MAX_MOBS 100

#define  DRAGON_SPRITE_FRAME_WIDTH 192
#define  DRAGON_SPRITE_FRAME_HEIGHT 176
#define  DRAGON_SPRITE_ROWS 1
#define  DRAGON_SPRITE_COLS 9
#define  DRAGON_SPEED 10

#endif // CONSTANTS_H
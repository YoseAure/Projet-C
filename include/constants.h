#ifndef CONSTANTS_H
#define CONSTANTS_H

// general
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900

#define TILE_SIZE 30

// Maps
#define WORLD_WIDTH 135000
#define WORLD_HEIGHT 900

typedef enum {
    EMPTY,
    GROUND,
    BRICK,
    COIN,
    ENEMY
} BlockType;

// Player partie mariobros
#define GRAVITY 1
#define JUMP_FORCE -20
#define JUMP_DELAY 500 // ms
#define MOVEMENT_SPEED 10
#define SCROLL_SPEED 10

#define SPRITE_WIDTH 64
#define SPRITE_HEIGHT 64
#define SPRITE_ROWS 46
#define SPRITE_COLS 13

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

#endif // CONSTANTS_H
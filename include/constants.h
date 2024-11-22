#ifndef CONSTANTS_H
#define CONSTANTS_H

// general
#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000

#define TILE_SIZE 20

// Maps
#define WORLD_WIDTH 800
#define WORLD_HEIGHT 600

typedef enum {
    EMPTY,
    GROUND,
    BRICK,
    COIN,
    ENEMY,
    PLAYER
} BlockType;

// Player part mariobros
#define GRAVITY 1
#define JUMP_FORCE -15
#define JUMP_DELAY 600 // ms
#define MOVEMENT_SPEED 5
#define SCROLL_SPEED 5

#endif // CONSTANTS_H
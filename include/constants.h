#ifndef CONSTANTS_H
#define CONSTANTS_H

// general
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define TILE_SIZE 20

// Maps
#define WORLD_WIDTH 6000
#define WORLD_HEIGHT 620

typedef enum {
    EMPTY,
    GROUND,
    BRICK,
    COIN,
    ENEMY,
    PLAYER
} BlockType;

// Player partie mariobros
#define GRAVITY 1
#define JUMP_FORCE -18
#define JUMP_DELAY 500 // ms
#define MOVEMENT_SPEED 6
#define SCROLL_SPEED 6

#endif // CONSTANTS_H
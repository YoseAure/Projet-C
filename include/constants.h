#ifndef CONSTANTS_H
#define CONSTANTS_H

// general
#define WINDOW_WIDTH 1400
#define WINDOW_HEIGHT 900

#define TILE_SIZE 30

// Maps
#define WORLD_WIDTH 9000
#define WORLD_HEIGHT 900

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
#define JUMP_FORCE -15
#define JUMP_DELAY 500 // ms
#define MOVEMENT_SPEED 10
#define SCROLL_SPEED 10

#endif // CONSTANTS_H
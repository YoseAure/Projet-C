#ifndef CONSTANTS_H
#define CONSTANTS_H

// general
#define WINDOW_WIDTH 1250 // 1850 // 1280
#define WINDOW_HEIGHT 900 // 1000 // 720

#define TILE_SIZE 30

// Maps
#define WORLD_WIDTH 13500
#define WORLD_HEIGHT 1500

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
    PESTY,
    BLACK,
    BLOCK_DRAGON,
    BLOCK_PRINCESS,
    PLAYER,
    SAPIN,
    HOUSE,
    CHEST,
    ROCK,
    PANEL,
    GRASS,
    EARTH,
    WATER,
    SOCKS,
    PLANCHER,
    PONTON,
    STORE,
    PLANCHESURFSHOP,
    NEXT_MAP,
    NEXT_LEVEL,
    PREVIOUS_MAP,
    PREVIOUS_LEVEL,
    DEATH,
    WIN
} BlockType;

typedef enum {
    DRAGON,
    PRINCESS,
    PESTYFLORE,
    PESTYFLORE2
} MobType;

// Player
#define GRAVITY 1
#define JUMP_FORCE -15
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


// Items
#define MAX_ITEMS 9

// Inventory
#define INVENTORY_DEFINED
#define INVENTORY_SIZE 3


#endif // CONSTANTS_H
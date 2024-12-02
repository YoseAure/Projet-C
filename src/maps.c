#include "../include/maps.h"
#include <stdio.h>
#include <stdlib.h>

extern Player player;

SDL_Texture *ground_texture = NULL;
SDL_Texture *brick_texture = NULL;
SDL_Texture *coin_texture = NULL;
SDL_Texture *enemy_texture = NULL;
SDL_Texture *player_texture = NULL;
SDL_Texture *background_texture = NULL;

bool load_block_textures(SDL_Renderer *renderer) {
    ground_texture = IMG_LoadTexture(renderer, "../assets/images/floor.png");
    if (!ground_texture) {
        printf("Error loading ground texture: %s\n", SDL_GetError());
        return false;
    }

    brick_texture = IMG_LoadTexture(renderer, "../assets/images/brick.png");
    if (!brick_texture) {
        printf("Error loading brick texture: %s\n", SDL_GetError());
        return false;
    }

    background_texture = IMG_LoadTexture(renderer, "../assets/images/background.jpeg");
    if (background_texture == NULL) {
        printf("Error loading background texture: %s\n", SDL_GetError());
    }

    coin_texture = IMG_LoadTexture(renderer, "../assets/images/coin.gif");
    if (!coin_texture) {
        printf("Error loading coin texture: %s\n", SDL_GetError());
        return false;
    }

    enemy_texture = IMG_LoadTexture(renderer, "../assets/images/enemy.gif");
    if (!enemy_texture) {
        printf("Error loading ennemy texture: %s\n", SDL_GetError());
        return false;
    }

    player_texture = IMG_LoadTexture(renderer, "../assets/sprites/naked-player.png");
    if (!player_texture) {
        printf("Error loading player texture: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

void free_block_textures() {
    if (ground_texture) {
        SDL_DestroyTexture(ground_texture);
        ground_texture = NULL;
    }
    if (brick_texture) {
        SDL_DestroyTexture(brick_texture);
        brick_texture = NULL;
    }
    if (coin_texture) {
        SDL_DestroyTexture(coin_texture);
        coin_texture = NULL;
    }
    if (enemy_texture) {
        SDL_DestroyTexture(enemy_texture);
        enemy_texture = NULL;
    }
    if (player_texture) {
        SDL_DestroyTexture(player_texture);
        player_texture = NULL;
    }
    if (background_texture) {
        SDL_DestroyTexture(background_texture);
        background_texture = NULL;
    }
}

void init_map(Map *map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            map->blocks[i][j] = EMPTY;
        }
    }
}

Map* load_map(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier de carte: %s\n", filename);
        return NULL;
    }

    int width = WORLD_WIDTH / TILE_SIZE;
    int height = WORLD_HEIGHT / TILE_SIZE;

    Map *map = malloc(sizeof(Map));
    if (map == NULL) {
        fclose(file);
        return NULL;
    }

    map->width = width;
    map->height = height;

    map->blocks = malloc(height * sizeof(BlockType*));
    if (map->blocks == NULL) {
        free(map);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < height; ++i) {
        map->blocks[i] = malloc(width * sizeof(BlockType));
        if (map->blocks[i] == NULL) {
            for (int j = 0; j < i; ++j) {
                free(map->blocks[j]);
            }
            free(map->blocks);
            free(map);
            fclose(file);
            return NULL;
        }
    }

    init_map(map);

    char line[width + 1];
    int row = 0;
    while (fgets(line, sizeof(line), file) && row < height) {
        for (int col = 0; col < width && line[col] != '\0'; col++) {
            switch (line[col]) {
                case 'G':
                    map->blocks[row][col] = GROUND;
                    break;
                case 'B':
                    map->blocks[row][col] = BRICK;
                    break;
                case 'C':
                    map->blocks[row][col] = COIN;
                    break;
                case 'E':
                    map->blocks[row][col] = ENEMY;
                    break;
                default:
                    map->blocks[row][col] = EMPTY;
                    break;
            }
        }
        row++;
    }

    fclose(file);
    return map;
}

void free_map(Map *map) {
    if (map == NULL) {
        return;
    }

    for (int i = 0; i < map->height; ++i) {
        free(map->blocks[i]);
    }
    free(map->blocks);
    free(map);
}

void render_map(SDL_Renderer *renderer, Map *map, int cameraX) {
    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    int background_width = window_width;
    int background_height = window_height;
    int background_x = -cameraX % background_width;
    int background_y = 0; // Position verticale de l'image de fond

    SDL_Rect backgroundRect = {background_x, background_y, background_width, background_height};
    SDL_RenderCopy(renderer, background_texture, NULL, &backgroundRect);

    if (background_x + background_width < window_width) {
        SDL_Rect backgroundRect2 = {background_x + background_width, background_y, background_width, background_height};
        SDL_RenderCopy(renderer, background_texture, NULL, &backgroundRect2);
    }

    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            SDL_Rect rect = { j * TILE_SIZE - cameraX, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            switch (map->blocks[i][j]) {
                case GROUND:
                    SDL_RenderCopy(renderer, ground_texture, NULL, &rect);
                    break;
                case BRICK:
                    SDL_RenderCopy(renderer, brick_texture, NULL, &rect);
                    break;
                case COIN:
                    SDL_RenderCopy(renderer, coin_texture, NULL, &rect);
                    break;
                case ENEMY:
                    SDL_RenderCopy(renderer, enemy_texture, NULL, &rect);
                    break;
                case EMPTY:
                default:
                    break;
            }
        }
    }
}

void reset_map(Map **map) {
    free_map(*map);
    *map = NULL;
    player.x = 5 * TILE_SIZE;
    player.y = 20 * TILE_SIZE;
}
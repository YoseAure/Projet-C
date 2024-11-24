#include "../include/maps.h"
#include <stdio.h>
#include <stdlib.h>

extern Player player;

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
                case 'P':
                    map->blocks[row][col] = PLAYER;
                    player.x = col * TILE_SIZE;
                    player.y = row * TILE_SIZE;
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
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            SDL_Rect rect = { j * TILE_SIZE - cameraX, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            switch (map->blocks[i][j]) {
                case GROUND:
                    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); // Gris
                    break;
                case BRICK:
                    SDL_SetRenderDrawColor(renderer, 205, 133, 63, 255); // Marron clair
                    break;
                case COIN:
                    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255); // Gold
                    break;
                case ENEMY:
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red
                    break;
                case PLAYER:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
                    break;
                case EMPTY:
                default:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black
                    break;
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_Rect player_rect = { player.x - cameraX, player.y, player.width, player.height };
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Blue
    SDL_RenderFillRect(renderer, &player_rect);
}

void reset_map(Map **map) {
    free_map(*map);
    *map = NULL;
    player.x = 5 * TILE_SIZE;
    player.y = 29 * TILE_SIZE;
}
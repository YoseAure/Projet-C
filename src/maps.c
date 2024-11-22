#include "../include/maps.h"
#include <stdio.h>
#include <stdlib.h>

BlockType** load_map(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Erreur lors de l'ouverture du fichier de carte: %s\n", filename);
        return NULL;
    }

    int width = WORLD_WIDTH / TILE_SIZE;
    int height = WORLD_HEIGHT / TILE_SIZE;

    BlockType **map = malloc(height * sizeof(BlockType*));
    for (int i = 0; i < height; ++i) {
        map[i] = malloc(width * sizeof(BlockType));
    }

    char line[width + 1];
    int row = 0;
    while (fgets(line, sizeof(line), file) && row < height) {
        for (int col = 0; col < width && line[col] != '\0'; col++) {
            switch (line[col]) {
                case 'G':
                    map[row][col] = GROUND;
                    break;
                case 'B':
                    map[row][col] = BRICK;
                    break;
                case 'C':
                    map[row][col] = COIN;
                    break;
                case 'E':
                    map[row][col] = ENEMY;
                    break;
                case 'P':
                    map[row][col] = PLAYER;
                    break;
                default:
                    map[row][col] = EMPTY;
                    break;
            }
        }
        row++;
    }

    fclose(file);
    return map;
}

void free_map(BlockType **map, int height) {
    for (int i = 0; i < height; ++i) {
        free(map[i]);
    }
    free(map);
}

void render_map(SDL_Renderer *renderer, BlockType **map, int width, int height) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            SDL_Rect rect = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            switch (map[i][j]) {
                case GROUND:
                    SDL_SetRenderDrawColor(renderer, 169, 169, 169, 255); // Gris moyen
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
}
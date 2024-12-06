#include "../include/maps.h"
#include <stdio.h>
#include <stdlib.h>

extern Player player;
extern SDL_Rect dragon_clips[MAX_SPRITE_ROWS][MAX_SPRITE_COLS];

extern Mob mobs[MAX_MOBS];
extern int mob_count;

SDL_Texture *ground_texture = NULL;
SDL_Texture *brick_texture = NULL;
SDL_Texture *pilar_texture = NULL;
SDL_Texture *torch_texture = NULL;
SDL_Texture *coin_texture = NULL;
SDL_Texture *enemy_texture = NULL;
SDL_Texture *player_texture = NULL;
SDL_Texture *background_texture = NULL;
SDL_Texture *dragon_texture = NULL;
SDL_Texture *princess_texture = NULL;
SDL_Texture *redFlag_texture = NULL;
SDL_Texture *socks_texture = NULL;

bool load_block_textures(SDL_Renderer *renderer) {
    ground_texture = IMG_LoadTexture(renderer, "../assets/images/ground-2.png");
    if (!ground_texture) {
        printf("Error loading ground texture: %s\n", SDL_GetError());
        return false;
    }

    brick_texture = IMG_LoadTexture(renderer, "../assets/images/brick-2.png");
    if (!brick_texture) {
        printf("Error loading brick texture: %s\n", SDL_GetError());
        return false;
    }

    pilar_texture = IMG_LoadTexture(renderer, "../assets/images/pilier.png");
    if (!pilar_texture) {
        printf("Error loading pilar texture: %s\n", SDL_GetError());
        return false;
    }

    torch_texture = IMG_LoadTexture(renderer, "../assets/images/torch.png");
    if (!torch_texture) {
        printf("Error loading torch texture: %s\n", SDL_GetError());
        return false;
    }

    redFlag_texture = IMG_LoadTexture(renderer, "../assets/images/red-flag.png");
    if (!redFlag_texture) {
        printf("Error loading red flag texture: %s\n", SDL_GetError());
        return false;
    }

    background_texture = IMG_LoadTexture(renderer, "../assets/images/background.jpeg");
    if (background_texture == NULL) {
        printf("Error loading background texture: %s\n", SDL_GetError());
    }

    coin_texture = IMG_LoadTexture(renderer, "../assets/images/sexwax.png");
    if (!coin_texture) {
        printf("Error loading coin texture: %s\n", SDL_GetError());
        return false;
    }

    enemy_texture = IMG_LoadTexture(renderer, "../assets/images/pestyflore.png");
    if (!enemy_texture) {
        printf("Error loading ennemy texture: %s\n", SDL_GetError());
        return false;
    }
    // dans blocks pour l'instant mais faudrait peut être le mettre dans un truc mobs
    player_texture = IMG_LoadTexture(renderer, "../assets/sprites/naked-player.png");
    if (!player_texture) {
        printf("Error loading player texture: %s\n", SDL_GetError());
        return false;
    }

    dragon_texture = IMG_LoadTexture(renderer, "../assets/sprites/dragon.png");
    if (!dragon_texture) {
        printf("Error loading dragon texture: %s\n", SDL_GetError());
        return false;
    }

    princess_texture = IMG_LoadTexture(renderer, "../assets/sprites/princess.png");
    if (!princess_texture) {
        printf("Error loading princess texture: %s\n", SDL_GetError());
        return false;
    }

    socks_texture = IMG_LoadTexture(renderer, "../assets/images/socks.png");
    if (!socks_texture) {
        printf("Error loading socks texture: %s\n", SDL_GetError());
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
    if (pilar_texture) {
        SDL_DestroyTexture(pilar_texture);
        pilar_texture = NULL;
    }
    if (torch_texture) {
        SDL_DestroyTexture(torch_texture);
        torch_texture = NULL;
    }
    if (redFlag_texture) {
        SDL_DestroyTexture(redFlag_texture);
        redFlag_texture = NULL;
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
    if (dragon_texture) {
        SDL_DestroyTexture(dragon_texture);
        dragon_texture = NULL;
    }
    if (princess_texture) {
        SDL_DestroyTexture(princess_texture);
        princess_texture = NULL;
    }
    if (socks_texture) {
        SDL_DestroyTexture(socks_texture);
        socks_texture = NULL;
    }
}

void init_map(Map *map) {
    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            map->blocks[i][j].type = EMPTY;
            map->blocks[i][j].isSolid = false;
        }
    }
}

// Map *load_map_from_csv(const char *filename)
// {
//     FILE *file = fopen(filename, "r");
//     if (!file)
//     {
//         printf("Erreur lors de l'ouverture du fichier pour la lecture: %s\n", filename);
//         return NULL;
//     }

//     Map *map = malloc(sizeof(Map));
//     if (!map)
//     {
//         fclose(file);
//         return NULL;
//     }

//     map->width = 0;
//     map->height = 0;

//     char line[1024];
//     while (fgets(line, sizeof(line), file))
//     {
//         if (map->width == 0)
//         {
//             char *token = strtok(line, ";");
//             while (token)
//             {
//                 map->width++;
//                 token = strtok(NULL, ";");
//             }
//         }
//         map->height++;
//     }

//     fseek(file, 0, SEEK_SET);

//     map->blocks = malloc(map->height * sizeof(BlockType *));
//     for (int i = 0; i < map->height; ++i)
//     {
//         map->blocks[i] = malloc(map->width * sizeof(BlockType));
//         fgets(line, sizeof(line), file);
//         char *token = strtok(line, ",");
//         for (int j = 0; j < map->width; ++j)
//         {
//             switch (token[0])
//             {
//             case 'B':
//                 map->blocks[i][j] = BRICK;
//                 break;
//             case 'G':
//                 map->blocks[i][j] = GROUND;
//                 break;
//             case 'C':
//                 map->blocks[i][j] = COIN;
//                 break;
//             case 'E':
//                 map->blocks[i][j] = ENEMY;
//                 break;
//             case 'N':
//                 map->blocks[i][j] = BLACK;
//                 break;
//             default:
//                 map->blocks[i][j] = EMPTY;
//                 break;
//             }
//             token = strtok(NULL, ",");
//         }
//     }

//     fclose(file);
//     return map;
// }

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

    map->blocks = malloc(height * sizeof(Block*));
    if (map->blocks == NULL) {
        free(map);
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < height; ++i) {
        map->blocks[i] = malloc(width * sizeof(Block));
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
            map->blocks[row][col].x = col * TILE_SIZE;
            map->blocks[row][col].y = row * TILE_SIZE;
            map->blocks[row][col].width = TILE_SIZE;
            map->blocks[row][col].height = TILE_SIZE;

            switch (line[col]) {
                case 'I':
                    map->blocks[row][col].type = INVISIBLE;
                    map->blocks[row][col].isSolid = true;
                    break;
                case 'J':
                    map->blocks[row][col].type = PLAYER;
                    map->blocks[row][col].isSolid = false;
                    player.x = col * TILE_SIZE;
                    player.y = row * TILE_SIZE;
                    player.width = TILE_SIZE;
                    player.height = TILE_SIZE * 1.5;
                    player.isJumping = false;
                    player.y_speed = 0;
                    player.lastJump_t = 0;
                    player.lastHit_t = 0;
                    player.life_points = 3;
                    player.total_life_points = 3;
                    player.coins_count = 0;
                    player.animation_row = IDLE;
                    player.texture = player_texture;
                    getSpriteClips(player.clips, PLAYER_SPRITE_ROWS, PLAYER_SPRITE_COLS, PLAYER_SPRITE_FRAME_WIDTH, PLAYER_SPRITE_FRAME_HEIGHT);
                    break;
                case 'G':
                    map->blocks[row][col].type = GROUND;
                    map->blocks[row][col].isSolid = true;
                    break;
                case 'B':
                    map->blocks[row][col].type = BRICK;
                    map->blocks[row][col].isSolid = true;
                    break;
                case 'V':
                    map->blocks[row][col].type = PILAR;
                    map->blocks[row][col].isSolid = true;
                    break;
                case 'T':
                    map->blocks[row][col].type = TORCH;
                    map->blocks[row][col].isSolid = true;
                    break;
                case 'F':
                    map->blocks[row][col].type = RED_FLAG;
                    map->blocks[row][col].isSolid = true;
                    break;
                case 'C':
                    map->blocks[row][col].width = TILE_SIZE * 2;
                    map->blocks[row][col].height = TILE_SIZE * 2;
                    map->blocks[row][col].type = COIN;
                    map->blocks[row][col].isSolid = false;
                    break;
                case 'E':
                    map->blocks[row][col].type = ENEMY;
                    map->blocks[row][col].isSolid = false;
                    if (mob_count < MAX_MOBS) {
                        mobs[mob_count].x = col * TILE_SIZE;
                        mobs[mob_count].y = row * TILE_SIZE;
                        mobs[mob_count].width = TILE_SIZE;
                        mobs[mob_count].height = TILE_SIZE;
                        mobs[mob_count].type = PESTYFLORE;
                        mobs[mob_count].animation_row = 0;
                        mobs[mob_count].texture = enemy_texture;
                        mobs[mob_count].direction = 1;
                        mobs[mob_count].x_speed = 0;
                        mobs[mob_count].y_speed = 0;
                        mobs[mob_count].initial_x = col * TILE_SIZE;
                        mobs[mob_count].initial_y = row * TILE_SIZE;
                        mob_count++;
                    }
                    break;
                case 'N':
                    map->blocks[row][col].type = BLACK;
                    map->blocks[row][col].isSolid = false;
                    break;
                case 'D':
                    map->blocks[row][col].type = BLOCK_DRAGON;
                    map->blocks[row][col].isSolid = false;
                    if (mob_count < MAX_MOBS) {
                        mobs[mob_count].x = col * TILE_SIZE;
                        mobs[mob_count].y = row * TILE_SIZE;
                        mobs[mob_count].width = TILE_SIZE * 6;
                        mobs[mob_count].height = TILE_SIZE * 6;
                        mobs[mob_count].type = DRAGON;
                        mobs[mob_count].animation_row = 0;
                        mobs[mob_count].texture = dragon_texture;
                        mobs[mob_count].direction = 1;
                        mobs[mob_count].x_speed = 0;
                        mobs[mob_count].y_speed = 0;
                        mobs[mob_count].initial_x = col * TILE_SIZE;
                        mobs[mob_count].initial_y = row * TILE_SIZE;
                        getSpriteClips(mobs[mob_count].clips, DRAGON_SPRITE_ROWS, DRAGON_SPRITE_COLS, DRAGON_SPRITE_FRAME_WIDTH, DRAGON_SPRITE_FRAME_HEIGHT);
                        mob_count++;
                    }
                    break;
                case 'P':
                    map->blocks[row][col].type = BLOCK_PRINCESS;
                    map->blocks[row][col].isSolid = false;
                    if (mob_count < MAX_MOBS) {
                        mobs[mob_count].x = col * TILE_SIZE;
                        mobs[mob_count].y = row * TILE_SIZE;
                        mobs[mob_count].width = TILE_SIZE;
                        mobs[mob_count].height = TILE_SIZE * 1.5;
                        mobs[mob_count].type = PRINCESS;
                        mobs[mob_count].animation_row = 13;
                        mobs[mob_count].texture = princess_texture;
                        mobs[mob_count].direction = 1;
                        mobs[mob_count].x_speed = 0;
                        mobs[mob_count].y_speed = 0;
                        mobs[mob_count].initial_x = col * TILE_SIZE;
                        mobs[mob_count].initial_y = row * TILE_SIZE;
                        getSpriteClips(mobs[mob_count].clips, PLAYER_SPRITE_ROWS, PLAYER_SPRITE_COLS, PLAYER_SPRITE_FRAME_WIDTH, PLAYER_SPRITE_FRAME_HEIGHT);
                        mob_count++;
                    }
                    break;
                case 'S':
                    map->blocks[row][col].width = TILE_SIZE * 1.5;
                    map->blocks[row][col].height = TILE_SIZE * 1.5;
                    map->blocks[row][col].type = SOCKS;
                    map->blocks[row][col].isSolid = false;
                    break;
                default:
                    map->blocks[row][col].type = EMPTY;
                    map->blocks[row][col].isSolid = false;
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

void render_map(SDL_Renderer *renderer, Map *map, int cameraX, int cameraY) {
    /*
    TODO: ajouter variable i et j pour charger les différentes map
    i : numéro du niveau
    j : numéro de la map
    idée : charger les maps dans un tableau de map ?
    idée : charger différentes musiques selon la map en utilisant la même logique avec i et j ?
    */
   
    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    int background_width = window_width;
    int background_height = window_height;
    int background_x = -cameraX % background_width;
    int background_y = 0;

    SDL_Rect backgroundRect = {background_x, background_y - cameraY, background_width, background_height};
    SDL_RenderCopy(renderer, background_texture, NULL, &backgroundRect);

    if (background_x + background_width < window_width) {
        SDL_Rect backgroundRect2 = {background_x + background_width, background_y - cameraY, background_width, background_height};
        SDL_RenderCopy(renderer, background_texture, NULL, &backgroundRect2);
    }

    for (int i = 0; i < map->height; ++i) {
        for (int j = 0; j < map->width; ++j) {
            SDL_Rect rect = { map->blocks[i][j].x - cameraX, map->blocks[i][j].y - cameraY, map->blocks[i][j].width, map->blocks[i][j].height };
            switch (map->blocks[i][j].type) {
                case GROUND:
                    SDL_RenderCopy(renderer, ground_texture, NULL, &rect);
                    break;
                case BRICK:
                    SDL_RenderCopy(renderer, brick_texture, NULL, &rect);
                    break;
                case PILAR:
                    SDL_RenderCopy(renderer, pilar_texture, NULL, &rect);
                    break;
                case TORCH:
                    SDL_RenderCopy(renderer, torch_texture, NULL, &rect);
                    break;
                case RED_FLAG:
                    SDL_RenderCopy(renderer, redFlag_texture, NULL, &rect);
                    break;
                case COIN:
                    SDL_RenderCopy(renderer, coin_texture, NULL, &rect);
                    break;
                case BLACK:
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderFillRect(renderer, &rect);
                    break;
                case SOCKS:
                    SDL_RenderCopy(renderer, socks_texture, NULL, &rect);
                    break;
                case INVISIBLE:
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
}
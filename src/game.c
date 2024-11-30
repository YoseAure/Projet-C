#include "../include/game.h"
#include "../include/menu.h"
#include "../include/settings.h"
#include "../include/maps.h"
#include "../include/sprite.h"

extern bool exit_program;
extern SDL_Texture *player_texture;
SDL_Rect player_clips[SPRITE_ROWS][SPRITE_COLS];
int current_frame = 0;
int animation_row = 24;
int previous_mouvement = 24;
Uint32 last_frame_time = 0;
const Uint32 FRAME_DELAY = 100;

bool mario_gameType = false;
bool pokemon_gameType = true;

Player player = {5 * TILE_SIZE,
                20 * TILE_SIZE, 
                TILE_SIZE, 
                TILE_SIZE * 1.5, 
                false, 
                0, 
                0};

bool keys[SDL_NUM_SCANCODES] = {false};

bool checkCollision(Player *player, SDL_Rect *obs) {
    if (player->x + player->width > obs->x && player->x < obs->x + obs->w) {
        if (player->y + player->height > obs->y && player->y < obs->y + obs->h) {
            return true;
        }
    }
    return false;
}

void handle_input(SDL_Event *e) {
    if (e->type == SDL_KEYDOWN) {
        keys[e->key.keysym.scancode] = true;
    } else if (e->type == SDL_KEYUP) {
        keys[e->key.keysym.scancode] = false;
    }
}

void update_player(BlockType **map, int width, int height, Uint32 currentTime) {
    bool moving = false;
    // animation_row = IDLE;

    if (keys[SDL_SCANCODE_LEFT]) {
        bool canMove = true;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (map[i][j] == BRICK || map[i][j] == GROUND) {
                    SDL_Rect obs = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                    if (player.x - MOVEMENT_SPEED < obs.x + obs.w &&
                        player.x + player.width - MOVEMENT_SPEED > obs.x &&
                        player.y + player.height > obs.y && player.y < obs.y + obs.h) {
                        canMove = false;
                        break;
                    }
                }
            }
        }
        if (canMove && player.x > 0) {
            player.x -= MOVEMENT_SPEED;
            animation_row = LEFT;
            moving = true;
        }
    }

    if (keys[SDL_SCANCODE_RIGHT]) {
        bool canMove = true;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (map[i][j] == BRICK || map[i][j] == GROUND) {
                    SDL_Rect obs = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                    if (player.x + player.width + MOVEMENT_SPEED > obs.x &&
                        player.x + MOVEMENT_SPEED < obs.x + obs.w &&
                        player.y + player.height > obs.y && player.y < obs.y + obs.h) {
                        canMove = false;
                        break;
                    }
                }
            }
        }
        if (canMove && player.x + player.width < width * TILE_SIZE) {
            player.x += MOVEMENT_SPEED;
            animation_row = RIGHT;
            moving = true;
        }
    }

    if (pokemon_gameType) {
        if (keys[SDL_SCANCODE_UP]) {
            bool canMove = true;
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    if (map[i][j] == BRICK || map[i][j] == GROUND) {
                        SDL_Rect obs = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                        if (player.y - MOVEMENT_SPEED < obs.y + obs.h &&
                            player.y + player.height - MOVEMENT_SPEED > obs.y &&
                            player.x + player.width > obs.x && player.x < obs.x + obs.w) {
                            canMove = false;
                            break;
                        }
                    }
                }
            }
            if (canMove && player.y > 0) {
                player.y -= MOVEMENT_SPEED;
                animation_row = UP;
                moving = true;
            }
        }

        if (keys[SDL_SCANCODE_DOWN]) {
            bool canMove = true;
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    if (map[i][j] == BRICK || map[i][j] == GROUND) {
                        SDL_Rect obs = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                        if (player.y + player.height + MOVEMENT_SPEED > obs.y &&
                            player.y + MOVEMENT_SPEED < obs.y + obs.h &&
                            player.x + player.width > obs.x && player.x < obs.x + obs.w) {
                            canMove = false;
                            break;
                        }
                    }
                }
            }
            if (canMove && player.y + player.height < height * TILE_SIZE) {
                player.y += MOVEMENT_SPEED;
                animation_row = DOWN;
                moving = true;
            }
        }
    }

    if (mario_gameType) {
        if (keys[SDL_SCANCODE_SPACE] && !player.isJumping) {
            if (currentTime - player.lastJump_t >= JUMP_DELAY) {
                player.y_speed = JUMP_FORCE;
                player.isJumping = true;
                player.lastJump_t = currentTime;
                animation_row = JUMP;
            }
        }

        player.y += player.y_speed;
        if (player.isJumping) {
            player.y_speed += GRAVITY;
        }

        bool onGround = false;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (map[i][j] == GROUND || map[i][j] == BRICK) {
                    SDL_Rect obs = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
                    if (checkCollision(&player, &obs)) {
                        if (player.y_speed > 0) {
                            player.y = obs.y - player.height;
                            player.y_speed = 0;
                            onGround = true;
                        }
                        if (player.y_speed < 0) {
                            player.y = obs.y + obs.h;
                            player.y_speed = 0;
                        }
                    }
                }
            }
        }

        if (onGround) {
            player.isJumping = false;
            player.y_speed = 0;
        } else {
            player.isJumping = true;
        }
    }

    if (!moving && !player.isJumping) {
        if (previous_mouvement == LEFT || previous_mouvement == IDLE_LEFT) {
            animation_row = IDLE_LEFT;
        } else if (previous_mouvement == RIGHT || previous_mouvement == IDLE_RIGHT) {
            animation_row = IDLE_RIGHT;
        } else if (previous_mouvement == UP || previous_mouvement == IDLE_UP) {
            animation_row = IDLE_UP;
        } else {
            animation_row = IDLE;
        }
    }
    //     FRAME_DELAY = 100;
    // } else {
    //     FRAME_DELAY = 100;
    // }

    // Animation du perso
    if (currentTime - last_frame_time >= FRAME_DELAY) {
        switch (animation_row) {
            case JUMP:
                current_frame = (current_frame + 1) % 4;
                break;
            case LEFT:
            case RIGHT:
            case UP:
            case DOWN:
                current_frame = (current_frame + 1) % 7;
                break;
            case IDLE:
            case IDLE_RIGHT:
            case IDLE_LEFT:
            case IDLE_UP:
                current_frame = (current_frame + 1) % 2;
                break;
            default:
                animation_row = IDLE;
                current_frame = (current_frame + 1) % 2;
                break;
        }
        last_frame_time = currentTime;
        previous_mouvement = animation_row;
    }
}

void render_player(SDL_Renderer *renderer, int cameraX) {
    SDL_Rect player_rect = { player.x - cameraX, player.y, player.width, player.height };
    SDL_SetRenderDrawColor(renderer, 0, 0, 139, 255); // Bleu foncé
    SDL_RenderFillRect(renderer, &player_rect);

    SDL_Rect sprite_rect = { player.x - cameraX + (player.width - SPRITE_WIDTH) / 2, player.y - (SPRITE_HEIGHT - player.height) + 2, SPRITE_WIDTH, SPRITE_HEIGHT };
    SDL_RenderCopy(renderer, princess_texture, &princess_clips[animation_row][current_frame], &sprite_rect);
}

void display_in_game_menu(SDL_Renderer *renderer, bool *return_to_main_menu, bool *resume_game) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 32);
    TTF_Font *font_large = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 36);
    TTF_Font *title_font = TTF_OpenFont("assets/fonts/mario-font-2.ttf", 72);

    if (!font || !title_font || !font_large) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char *options[] = {"Resume Game", "Main Menu", "Settings"};
    int selected = 0;
    bool quit = false;
    SDL_Event event;

    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    SDL_Surface *logo_surface = IMG_Load("assets/images/surf.png");
    if (!logo_surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *logo_texture = SDL_CreateTextureFromSurface(renderer, logo_surface);
    SDL_FreeSurface(logo_surface);

    Mix_Chunk *menu_selection_sound = Mix_LoadWAV("assets/audio/menu-selection.mp3");
    if (!menu_selection_sound) {
        printf("Erreur Mix_LoadWAV: %s\n", Mix_GetError());
        exit_program = true;
        quit = true;
    }

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *return_to_main_menu = true;
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected - 1 + 3) % 3;
                        if (sound_effects_enabled) {
                            Mix_PlayChannel(-1, menu_selection_sound, 0);
                        }
                        break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % 3;
                        if (sound_effects_enabled) {
                            Mix_PlayChannel(-1, menu_selection_sound, 0);
                        }
                        break;
                    case SDLK_RETURN:
                        if (selected == 0) {
                            *resume_game = true;
                            quit = true;
                        } else if (selected == 1) {
                            *return_to_main_menu = true;
                            quit = true;
                        } else if (selected == 2) {
                            settings(renderer);
                        }
                        break;
                    case SDLK_ESCAPE:
                        *resume_game = true;
                        quit = true;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // titre : Game Paused
        SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, "Game Paused", white);
        SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);

        int title_width = title_surface->w;
        int title_height = title_surface->h;
        int title_x = (window_width - title_width) / 2;
        int title_y = (window_height / 4) - (title_height / 2);

        SDL_Rect title_dest = {title_x, title_y, title_width, title_height};
        SDL_RenderCopy(renderer, title_texture, NULL, &title_dest);

        SDL_FreeSurface(title_surface);
        SDL_DestroyTexture(title_texture);

        // options
        for (int i = 0; i < 3; ++i) {
            SDL_Color color = (i == selected) ? yellow : white;
            TTF_Font *current_font = (i == selected) ? font_large : font;

            SDL_Surface *surface = TTF_RenderText_Solid(current_font, options[i], color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            int text_width = surface->w;
            int text_height = surface->h;
            int x = (window_width - text_width) / 2;
            int y = (window_height / 2) + (i * (text_height + 20));

            SDL_Rect dest = {x, y, text_width, text_height};
            SDL_RenderCopy(renderer, texture, NULL, &dest);

            if (i == selected) {
                render_logo(renderer, logo_texture, y, x);
            }

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    Mix_FreeChunk(menu_selection_sound);
    SDL_DestroyTexture(logo_texture);
    TTF_CloseFont(font);
    TTF_CloseFont(font_large);
    TTF_CloseFont(title_font);
}

void start_game(SDL_Renderer *renderer) {
    bool quit_game = false;
    bool return_to_main_menu = false;
    bool in_game_menu = false;
    bool resume_game = false;
    SDL_Event event;

    load_block_textures(renderer);
    if (!load_block_textures(renderer)) {
        printf("Erreur lors du chargement des textures des blocs\n");
        return;
    }

    Map *map = load_map("assets/maps/map-1.0.txt");
    if (!map) {
        printf("Erreur lors du chargement de la carte\n");
        return;
    }

    princess_texture = loadTexture("assets/sprites/naked-player.png", renderer);
    if (!princess_texture) {
        printf("Erreur lors du chargement de la texture du joueur: %s\n", SDL_GetError());
        return;
    }
    getSpriteClips(princess_clips, SPRITE_ROWS, SPRITE_COLS);

    int cameraX = 0;
    Uint32 lastTime = 0;

    while (!quit_game && !return_to_main_menu) {
        Uint32 currentTime = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit_game = true;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                handle_input(&event);
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    in_game_menu = !in_game_menu;
                }
            }
        }

        if (!in_game_menu) {
            update_player(map->blocks, map->width, map->height, currentTime);

            if (player.x > cameraX + WINDOW_WIDTH / 2 && cameraX + WINDOW_WIDTH < WORLD_WIDTH) {
                cameraX += SCROLL_SPEED;
            } else if (player.x < cameraX + WINDOW_WIDTH / 2 && cameraX > 0) {
                cameraX -= SCROLL_SPEED;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            render_map(renderer, map, cameraX);
            render_player(renderer, cameraX);
        } else {
            display_in_game_menu(renderer, &return_to_main_menu, &resume_game);
            if (resume_game) {
                in_game_menu = false;
                resume_game = false;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 fps
    }

    if (return_to_main_menu) {
        return_to_main_menu = false;
        reset_map(&map);
        display_main_menu(renderer);
    } else {
        free_map(map);
    }

    free_block_textures();
}

#include "../include/game.h"

extern bool exit_program;
extern SDL_Texture *player_texture;
extern SDL_Texture *dragon_texture;
extern SDL_Texture *princess_texture;
SDL_Rect player_clips[MAX_SPRITE_ROWS][MAX_SPRITE_COLS];
SDL_Rect dragon_clips[MAX_SPRITE_ROWS][MAX_SPRITE_COLS];
SDL_Rect princess_clips[MAX_SPRITE_ROWS][MAX_SPRITE_COLS];

Mob mobs[MAX_MOBS];
int mob_count = 0;

int current_frame = 0;
int current_dragon_frame = 0;
int current_princess_frame = 0;

int animation_row = 24;
int previous_mouvement = 24;
Uint32 last_frame_time = 0;
const Uint32 FRAME_DELAY = 100;

bool mario_gameType = true;
bool pokemon_gameType = false;

Player player;

bool keys[SDL_NUM_SCANCODES] = {false};

void reset_game_state() {
    mob_count = 0;
    for (int i = 0; i < MAX_MOBS; i++) {
        mobs[i].x = 0;
        mobs[i].y = 0;
        mobs[i].width = 0;
        mobs[i].height = 0;
        mobs[i].type = 0;
        mobs[i].animation_row = 0;
        mobs[i].texture = NULL;
        mobs[i].current_frame = 0;
    }
}

void reset_keys() {
    for (int i = 0; i < SDL_NUM_SCANCODES; ++i) {
    keys[i] = false;
    }
}

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

void update_player(Block **map, int width, int height, Uint32 currentTime) {
    bool moving = false;

    if (mario_gameType && !pokemon_gameType) {
        player.height = TILE_SIZE * 1.5;
    } else if (pokemon_gameType && !mario_gameType) {
        player.height = TILE_SIZE;
    }

    if (keys[SDL_SCANCODE_LEFT]) {
        bool canMove = true;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (map[i][j].isSolid) {
                    SDL_Rect obs = { map[i][j].x, map[i][j].y, map[i][j].width, map[i][j].height };
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
            player.animation_row = LEFT;
            moving = true;
        }
    }

    if (keys[SDL_SCANCODE_RIGHT]) {
        bool canMove = true;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (map[i][j].isSolid) {
                    SDL_Rect obs = { map[i][j].x, map[i][j].y, map[i][j].width, map[i][j].height };
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
            player.animation_row = RIGHT;
            moving = true;
        }
    }

    if (pokemon_gameType) {
        if (keys[SDL_SCANCODE_UP]) {
            bool canMove = true;
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    if (map[i][j].isSolid) {
                        SDL_Rect obs = { map[i][j].x, map[i][j].y, map[i][j].width, map[i][j].height };
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
                player.animation_row = UP;
                moving = true;
            }
        }

        if (keys[SDL_SCANCODE_DOWN]) {
            bool canMove = true;
            for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                    if (map[i][j].isSolid) {
                        SDL_Rect obs = { map[i][j].x, map[i][j].y, map[i][j].width, map[i][j].height };
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
                player.animation_row = DOWN;
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
                player.animation_row = JUMP;
            }
        }

        player.y += player.y_speed;
        if (player.isJumping) {
            player.y_speed += GRAVITY;
        }

        bool onGround = false;
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                if (map[i][j].isSolid) {
                    SDL_Rect obs = { map[i][j].x, map[i][j].y, map[i][j].width, map[i][j].height };
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
                SDL_Rect obs = { map[i][j].x, map[i][j].y, map[i][j].width, map[i][j].height };
                if (map[i][j].type == ENEMY && checkCollision(&player, &obs)) {
                    if (currentTime - player.lastHit_t >= 1000) {
                        player.lastHit_t = currentTime;
                        player.life_points--;
                    }
                } else if (map[i][j].type == COIN && checkCollision(&player, &obs)) {
                    player.coins_count++;
                    map[i][j].type = EMPTY;
                }
            }
        }

        for (int i = 0; i < mob_count; i++) {
            if (mobs[i].type != PRINCESS) {
                SDL_Rect mob_rect = { mobs[i].x, mobs[i].y, mobs[i].width, mobs[i].height };
                if (checkCollision(&player, &mob_rect)) {
                    if (currentTime - player.lastHit_t >= 1000) {
                        player.lastHit_t = currentTime;
                        player.life_points--;
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
            player.animation_row = IDLE_LEFT;
        } else if (previous_mouvement == RIGHT || previous_mouvement == IDLE_RIGHT) {
            player.animation_row = IDLE_RIGHT;
        } else if (previous_mouvement == UP || previous_mouvement == IDLE_UP) {
            player.animation_row = IDLE_UP;
        } else {
            player.animation_row = IDLE;
        }
    }

    if (currentTime - last_frame_time >= FRAME_DELAY) {
        switch (player.animation_row) {
            case JUMP:
                player.current_frame = (player.current_frame + 1) % 4;
                break;
            case LEFT:
            case RIGHT:
            case UP:
            case DOWN:
                player.current_frame = (player.current_frame + 1) % 7;
                break;
            case IDLE:
            case IDLE_RIGHT:
            case IDLE_LEFT:
            case IDLE_UP:
                player.current_frame = (player.current_frame + 1) % 2;
                break;
            default:
                player.animation_row = IDLE;
                player.current_frame = (player.current_frame + 1) % 2;
                break;
        }
        last_frame_time = currentTime;
        previous_mouvement = player.animation_row;
    }
}

void animate_mobs(int width, int height, Uint32 currentTime) {
    static Uint32 last_mob_frame_time = 0;
    if (currentTime - last_mob_frame_time >= FRAME_DELAY) {
        for (int i = 0; i < mob_count; i++) {
            switch (mobs[i].type) {
                case DRAGON:
                    mobs[i].current_frame = (mobs[i].current_frame + 1) % DRAGON_SPRITE_COLS;
                    break;
                case PRINCESS:
                    mobs[i].current_frame = (mobs[i].current_frame + 1) % 1 + 5;
                    break;
                default:
                    break;
            }
        }
        last_mob_frame_time = currentTime;
    }
}

void render_player(SDL_Renderer *renderer, int cameraX) {
    SDL_Rect sprite_rect = { player.x - cameraX + (player.width - PLAYER_SPRITE_FRAME_WIDTH) / 2, player.y - (PLAYER_SPRITE_FRAME_HEIGHT - player.height) + 2, PLAYER_SPRITE_FRAME_WIDTH, PLAYER_SPRITE_FRAME_HEIGHT };
    SDL_RenderCopy(renderer, player.texture, &player.clips[player.animation_row][player.current_frame], &sprite_rect);
}

void render_mobs(SDL_Renderer *renderer, int cameraX) {
    for (int i = 0; i < mob_count; i++) {
        SDL_Rect mob_rect;

        switch (mobs[i].type) {
            case DRAGON:
                mob_rect = (SDL_Rect){mobs[i].x - cameraX, mobs[i].y, mobs[i].width, mobs[i].height};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
                SDL_RenderFillRect(renderer, &mob_rect);
                if (mobs[i].direction == 0) {
                    SDL_RenderCopyEx(renderer, mobs[i].texture, &mobs[i].clips[mobs[i].animation_row][mobs[i].current_frame], &mob_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
                } else {
                    SDL_RenderCopy(renderer, mobs[i].texture, &mobs[i].clips[mobs[i].animation_row][mobs[i].current_frame], &mob_rect);
                }
                break;
            case PRINCESS:
                mob_rect = (SDL_Rect){mobs[i].x - cameraX + (mobs[i].width - PLAYER_SPRITE_FRAME_WIDTH) / 2, mobs[i].y - (PLAYER_SPRITE_FRAME_HEIGHT - mobs[i].height) - 5, PLAYER_SPRITE_FRAME_WIDTH, PLAYER_SPRITE_FRAME_HEIGHT};
                SDL_Rect princess_background_rect = {mob_rect.x, mob_rect.y, mob_rect.w, mob_rect.h};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
                SDL_RenderFillRect(renderer, &princess_background_rect);
                SDL_RenderCopy(renderer, mobs[i].texture, &mobs[i].clips[mobs[i].animation_row][mobs[i].current_frame], &mob_rect);
                break;
            default:
                break;
        }
    }
}

void render_coin_count(SDL_Renderer *renderer, Player *player) {
    
    // ToDo : centraliser le chargement des polices ??

    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    char coin_text[20];
    sprintf(coin_text, "Sexwax: %d", player->coins_count);

    SDL_Surface *surface = TTF_RenderText_Solid(font, coin_text, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_Rect dest = {WINDOW_WIDTH - text_width - 10, 10, text_width, text_height};

    SDL_Rect background_rect = {dest.x - 5, dest.y - 5, text_width + 10, text_height + 10};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir
    SDL_RenderFillRect(renderer, &background_rect);

    SDL_RenderCopy(renderer, texture, NULL, &dest);

    // vraiment besoin de libérer en boucle ??
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void display_in_game_menu(SDL_Renderer *renderer, Player *player, bool *return_to_main_menu, bool *quit_game , bool *resume_game) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 32);
    TTF_Font *font_large = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 36);
    TTF_Font *title_font = TTF_OpenFont("assets/fonts/mario-font-2.ttf", 72);

    if (!font || !title_font || !font_large) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char *options[3];
    if (player->life_points > 0) {
        options[0] = "Resume Game";
        options[1] = "Main Menu";
        options[2] = "Settings";
    } else {
        options[0] = "Restart Game";
        options[1] = "Main Menu";
        options[2] = "Quit";
    }

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
                            if (player->life_points > 0) {
                                *resume_game = true;
                                reset_keys();
                                quit = true;
                            } else {
                                start_game(renderer);
                                quit = true;
                            }
                        } else if (selected == 1) {
                            *return_to_main_menu = true;
                            quit = true;
                        } else if (selected == 2) {
                            if (player->life_points > 0) {
                                settings(renderer);
                            } else {
                                *return_to_main_menu = true;
                                *quit_game = true;
                                exit_program = true;
                                quit = true;
                            }
                        }
                        break;
                    case SDLK_ESCAPE:
                        *resume_game = true;
                        reset_keys();
                        quit = true;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, player->life_points > 0 ? "Game Paused" : "Game Over" , white);
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

void update_mobs(Uint32 currentTime) {
    static Uint32 last_mob_update_time = 0;
    if (currentTime - last_mob_update_time >= FRAME_DELAY) {
        for (int i = 0; i < mob_count; i++) {
            switch (mobs[i].type) {
                case DRAGON:
                    if (mobs[i].direction == 0) {
                        mobs[i].x += DRAGON_SPEED;
                        if (mobs[i].x >= mobs[i].initial_x + 20 * TILE_SIZE) {
                            mobs[i].direction = 1;
                        }
                    } else {
                        mobs[i].x -= DRAGON_SPEED;
                        if (mobs[i].x <= mobs[i].initial_x - 20 * TILE_SIZE) {
                            mobs[i].direction = 0;
                        }
                    }
                    break;
                default:
                    break;
            }
        }
        last_mob_update_time = currentTime;
    }
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

    reset_game_state();
    reset_keys();

    Map *map = load_map("assets/maps/map-1.0.txt");
    if (!map) {
        printf("Erreur lors du chargement de la carte\n");
        return;
    }

    player.texture = loadTexture("assets/sprites/surfer-player.png", renderer);
    if (!player.texture) {
        printf("Erreur lors du chargement de la texture du joueur: %s\n", SDL_GetError());
        return;
    }

    int cameraX = player.x - WINDOW_WIDTH / 2;

    while (!quit_game && !return_to_main_menu) {
        Uint32 currentTime = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit_game = true;
                return_to_main_menu = true;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                handle_input(&event);
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    in_game_menu = !in_game_menu;
                }
            }
        }

        if (!in_game_menu) {
            update_player(map->blocks, map->width, map->height, currentTime);
            update_mobs(currentTime);
            animate_mobs(map->width, map->height, currentTime);

            if (player.x > cameraX + WINDOW_WIDTH / 2 && cameraX + WINDOW_WIDTH < WORLD_WIDTH) {
                cameraX += SCROLL_SPEED;
            } else if (player.x < cameraX + WINDOW_WIDTH / 2 && cameraX > 0) {
                cameraX -= SCROLL_SPEED;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (player.life_points > 0) {
                render_map(renderer, map, cameraX);
                render_player(renderer, cameraX);
                render_mobs(renderer, cameraX);
                render_coin_count(renderer, &player);
            } else {
                display_in_game_menu(renderer, &player, &return_to_main_menu, &quit_game, &resume_game);
            }
        } else {
            display_in_game_menu(renderer, &player, &return_to_main_menu, &quit_game, &resume_game);
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

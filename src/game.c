#include "../include/game.h"
#include "../include/speech.h"

extern bool exit_program;
extern SDL_Texture *coin_texture;
extern SDL_Texture *win_background;
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

Inventory player_inventory = { .item_count = 0 };

bool quit_game = false;
bool show_inventory = false;
bool new_map = false;
bool start_speech = false;
int speech_index = 0;
bool surfboard = false;
bool surfsheet = false;
SDL_Texture *surfer = NULL;

int current_level = 1;
int current_map = 0;

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
        if (e->key.keysym.scancode == SDL_SCANCODE_I) {
            show_inventory = !show_inventory;
        }
    } else if (e->type == SDL_KEYUP) {
        keys[e->key.keysym.scancode] = false;
    }
}

SDL_Texture* load_texture(const char *file, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(file);
    if (!surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}

void update_player(SDL_Renderer *renderer, Map *map, int width, int height, Uint32 currentTime) {
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
                if (map->blocks[i][j].isSolid) {
                    SDL_Rect obs = { map->blocks[i][j].x, map->blocks[i][j].y, map->blocks[i][j].width, map->blocks[i][j].height };
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
                if (map->blocks[i][j].isSolid) {
                    SDL_Rect obs = { map->blocks[i][j].x, map->blocks[i][j].y, map->blocks[i][j].width, map->blocks[i][j].height };
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
                    if (map->blocks[i][j].isSolid) {
                        SDL_Rect obs = { map->blocks[i][j].x, map->blocks[i][j].y, map->blocks[i][j].width, map->blocks[i][j].height };
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
                    if (map->blocks[i][j].isSolid) {
                        SDL_Rect obs = { map->blocks[i][j].x, map->blocks[i][j].y, map->blocks[i][j].width, map->blocks[i][j].height };
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
                if (map->blocks[i][j].isSolid) {
                    SDL_Rect obs = { map->blocks[i][j].x, map->blocks[i][j].y, map->blocks[i][j].width, map->blocks[i][j].height };
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
                SDL_Rect obs = { map->blocks[i][j].x, map->blocks[i][j].y, map->blocks[i][j].width, map->blocks[i][j].height };
                if (map->blocks[i][j].type == ENEMY && checkCollision(&player, &obs)) {
                    if (currentTime - player.lastHit_t >= 1000) {
                        player.lastHit_t = currentTime;
                        player.life_points--;
                        play_life_loss_sound();
                        // play_sound("assets/audio/hp-loss.mp3");
                    }
                } else if (map->blocks[i][j].type == COIN && checkCollision(&player, &obs)) {
                    player.coins_count++;
                    map->blocks[i][j].type = EMPTY;
                    play_pop_sound();
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

    for (int i = 0; i < mob_count; i++) {
        if (mobs[i].type != PRINCESS) {
            SDL_Rect mob_rect = {mobs[i].x, mobs[i].y, mobs[i].width, mobs[i].height};
            if (checkCollision(&player, &mob_rect)) {
                if (currentTime - player.lastHit_t >= 1000) {
                    player.lastHit_t = currentTime;
                    player.life_points--;
                    play_life_loss_sound();
                    // play_sound("assets/audio/hp-loss.mp3");
                }
            }
        }
    }

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            SDL_Rect obs = { map->blocks[i][j].x, map->blocks[i][j].y, map->blocks[i][j].width, map->blocks[i][j].height };
            if (checkCollision(&player, &obs)) {
                switch (map->blocks[i][j].type) {
                    case NEXT_MAP:
                        current_map++;
                        if (current_map == 2) {
                            Mix_HaltMusic();
                            Mix_Music *new_music = Mix_LoadMUS("assets/audio/surfshop.mp3");
                            if (!new_music) {
                                printf("Erreur Mix_LoadMUS: %s\n", Mix_GetError());
                            } else {
                                Mix_PlayMusic(new_music, -1);
                            }
                        } else {
                            Mix_HaltMusic();
                            Mix_Music *new_music = Mix_LoadMUS("assets/audio/background-music-2.mp3");
                            if (!new_music) {
                                printf("Erreur Mix_LoadMUS: %s\n", Mix_GetError());
                            } else {
                                Mix_PlayMusic(new_music, -1);
                            }
                        }
                        new_map = true;
                        if (current_map > 0 && current_map < 5) {
                            pokemon_gameType = true;
                            mario_gameType = false;
                        } else {
                            pokemon_gameType = false;
                            mario_gameType = true;
                        }
                        break;
                    case NEXT_LEVEL:
                        current_level++;
                        current_map = 0;
                        new_map = true;
                        if (current_map > 0) {
                            pokemon_gameType = true;
                            mario_gameType = false;
                        } else {
                            pokemon_gameType = false;
                            mario_gameType = true;
                        }
                        break;
                    case PREVIOUS_MAP:
                        current_map--;
                        new_map = true;
                        if (current_map > 0) {
                            pokemon_gameType = true;
                            mario_gameType = false;
                        } else {
                            pokemon_gameType = false;
                            mario_gameType = true;
                        }
                        break;
                    case PREVIOUS_LEVEL:
                        current_level--;
                        current_map = 0;
                        new_map = true;
                        if (current_map > 0) {
                            pokemon_gameType = true;
                            mario_gameType = false;
                        } else {
                            pokemon_gameType = false;
                            mario_gameType = true;
                        }
                        break;
                    case SOCKS:
                        if (player_inventory.item_count < MAX_ITEMS) {
                            current_map++;
                            new_map = true;
                            if (current_map > 0 && current_map < 5) {
                                pokemon_gameType = true;
                                mario_gameType = false;
                            } else {
                                pokemon_gameType = false;
                                mario_gameType = true;
                            }
                            SDL_Texture *socks_texture = load_texture("assets/images/socks.png", renderer);
                            if (socks_texture) {
                                Item socks = { "Socks", socks_texture };
                                player_inventory.items[player_inventory.item_count] = socks;
                                player_inventory.item_count++;
                                map->blocks[i][j].type = EMPTY;
                            }
                        }
                        break;
                    case STORE:
                        speech_index = 0;
                        start_speech = true;
                        break;
                    case DEATH:
                        player.life_points = 0;
                        break;
                    case WIN:
                        player.life_points = -1;
                        break;
                    default:
                        break;
                }
            }
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

void render_player(SDL_Renderer *renderer, int cameraX, int cameraY) {
    SDL_Rect sprite_rect = { player.x - cameraX + (player.width - PLAYER_SPRITE_FRAME_WIDTH) / 2, player.y - cameraY + (player.height - PLAYER_SPRITE_FRAME_HEIGHT) + 2, PLAYER_SPRITE_FRAME_WIDTH, PLAYER_SPRITE_FRAME_HEIGHT };
    SDL_RenderCopy(renderer, player.texture, &player.clips[player.animation_row][player.current_frame], &sprite_rect);
}

void render_mobs(SDL_Renderer *renderer, int cameraX, int cameraY) {
    for (int i = 0; i < mob_count; i++) {
        SDL_Rect mob_rect;

        switch (mobs[i].type) {
            case DRAGON:
                mob_rect = (SDL_Rect){mobs[i].x - cameraX, mobs[i].y - cameraY, mobs[i].width, mobs[i].height};
                // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
                // SDL_RenderFillRect(renderer, &mob_rect);
                if (mobs[i].direction == 0) {
                    SDL_RenderCopyEx(renderer, mobs[i].texture, &mobs[i].clips[mobs[i].animation_row][mobs[i].current_frame], &mob_rect, 0, NULL, SDL_FLIP_HORIZONTAL);
                } else {
                    SDL_RenderCopy(renderer, mobs[i].texture, &mobs[i].clips[mobs[i].animation_row][mobs[i].current_frame], &mob_rect);
                }
                break;
            case PRINCESS:
                mob_rect = (SDL_Rect){mobs[i].x - cameraX + (mobs[i].width - PLAYER_SPRITE_FRAME_WIDTH) / 2, mobs[i].y - cameraY + (mobs[i].height - PLAYER_SPRITE_FRAME_HEIGHT) - 5, PLAYER_SPRITE_FRAME_WIDTH, PLAYER_SPRITE_FRAME_HEIGHT};
                SDL_Rect princess_background_rect = {mob_rect.x, mob_rect.y, mob_rect.w, mob_rect.h};
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir
                SDL_RenderFillRect(renderer, &princess_background_rect);
                SDL_RenderCopy(renderer, mobs[i].texture, &mobs[i].clips[mobs[i].animation_row][mobs[i].current_frame], &mob_rect);
                break;
            case PESTYFLORE:
                mob_rect = (SDL_Rect){mobs[i].x - cameraX, mobs[i].y - cameraY - TILE_SIZE, mobs[i].width * 2, mobs[i].height * 2};
                // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Rouge
                // SDL_RenderFillRect(renderer, &mob_rect);
                SDL_RenderCopy(renderer, mobs[i].texture, NULL, &mob_rect);
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
    SDL_Rect dest = {WINDOW_WIDTH - text_width - 10, 2 * TILE_SIZE / 3, text_width, text_height};

    // SDL_Rect background_rect = {dest.x - 5, dest.y - 5, text_width + 10, text_height + 10};
    // SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir
    // SDL_RenderFillRect(renderer, &background_rect);

    int icon_size = 50;
    SDL_Rect icon_dest = {WINDOW_WIDTH - text_width - icon_size - 20, TILE_SIZE / 3, icon_size, icon_size};
    SDL_RenderCopy(renderer, coin_texture, NULL, &icon_dest);
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    // vraiment besoin de libérer en boucle ??
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void render_player_life(SDL_Renderer *renderer, Player *player) {
    SDL_Surface *heart_surface = IMG_Load("assets/images/heart.png");
    if (!heart_surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *heart_texture = SDL_CreateTextureFromSurface(renderer, heart_surface);
    SDL_FreeSurface(heart_surface);

    SDL_Surface *grey_heart_surface = IMG_Load("assets/images/grey-heart.png");
    if (!grey_heart_surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        SDL_DestroyTexture(heart_texture);
        return;
    }
    SDL_Texture *grey_heart_texture = SDL_CreateTextureFromSurface(renderer, grey_heart_surface);
    SDL_FreeSurface(grey_heart_surface);

    int heart_width = TILE_SIZE;
    int heart_height = TILE_SIZE;
    for (int i = 0; i < player->total_life_points; ++i) {
        SDL_Rect heart_rect = {10 + i * (heart_width + 5), 10, heart_width, heart_height};
        if (i < player->life_points) {
            SDL_RenderCopy(renderer, heart_texture, NULL, &heart_rect);
        } else {
            SDL_RenderCopy(renderer, grey_heart_texture, NULL, &heart_rect);
        }
    }

    SDL_DestroyTexture(heart_texture);
    SDL_DestroyTexture(grey_heart_texture);
}

void display_in_game_menu(SDL_Renderer *renderer, Player *player, bool *return_to_main_menu , bool *resume_game) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 32);
    TTF_Font *font_large = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 36);
    TTF_Font *title_font = TTF_OpenFont("assets/fonts/mario-font-2.ttf", 72);

    if (!font || !title_font || !font_large) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color black = {0, 0, 0, 255};
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

    SDL_Texture *win_background_texture = load_texture("assets/images/win.png", renderer);
    if (!win_background_texture) {
        printf("Erreur lors du chargement de l'image de fond\n");
        return;
    }

    SDL_Texture *background_texture = load_texture("assets/images/img_game_menu.jpeg", renderer);
    if (!background_texture) {
        printf("Erreur lors du chargement de l'image de fond\n");
        return;
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
                            quit_game = true;
                            quit = true;
                        } else if (selected == 2) {
                            if (player->life_points > 0) {
                                settings(renderer);
                            } else {
                                *return_to_main_menu = true;
                                quit_game = true;
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
        if (player->life_points >= 0) {
            SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        } else {
            SDL_RenderCopy(renderer, win_background_texture, NULL, NULL);
        }

        SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, player->life_points > 0 ? "Game Paused" : "Game Over" , black);
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
            SDL_Color color = (i == selected) ? yellow : black;
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
    SDL_DestroyTexture(background_texture);
    SDL_DestroyTexture(win_background_texture);
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

void render_fade(SDL_Renderer *renderer, int alpha) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, alpha);
    SDL_RenderFillRect(renderer, NULL);
}

void blacken(SDL_Renderer *renderer, int duration) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_Rect rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 85);
    for (int i = 0; i < duration; i++) {
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }
}

void blackout(SDL_Renderer *renderer) {
    blacken(renderer, 25);
}

void dim(SDL_Renderer *renderer) {
    blacken(renderer, 10);
}

void render_speech_bubble(SDL_Renderer *renderer, const char *message) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color black = {0, 0, 0, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, message, black);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_Rect dest = {(WINDOW_WIDTH - text_width) / 2, (WINDOW_HEIGHT - text_height) / 2, text_width, text_height};

    SDL_Texture *bubble_texture = load_texture("assets/images/speech_bubble.png", renderer);
    if (!bubble_texture) {
        printf("Erreur lors du chargement de l'image de la bulle de dialogue\n");
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect background_rect = {dest.x - 25, dest.y - 10, text_width + 80, text_height + 70};
    SDL_RenderCopy(renderer, bubble_texture, NULL, &background_rect);

    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(bubble_texture);
    TTF_CloseFont(font);
}

void render_inventory(SDL_Renderer *renderer, Inventory *inventory) {
    int inventory_size = 3;
    int item_size = 64;
    int padding = 10;
    int background_padding = 30;
    int start_x = (WINDOW_WIDTH - (inventory_size * item_size + (inventory_size - 1) * padding)) / 2;
    int start_y = (WINDOW_HEIGHT - (inventory_size * item_size + (inventory_size - 1) * padding)) / 2;

    SDL_Rect background_rect = {
        start_x - background_padding,
        start_y - background_padding,
        inventory_size * (item_size + padding) - padding + 2 * background_padding,
        inventory_size * (item_size + padding) - padding + 2 * background_padding};
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &background_rect);

    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, "Inventaire", white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect text_rect = {start_x - background_padding - -5, start_y - background_padding - 1, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &text_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
    for (int i = 0; i < inventory_size; ++i) {
        for (int j = 0; j < inventory_size; ++j)
        {
            SDL_Rect cell_rect = {
                start_x + j * (item_size + padding),
                start_y + i * (item_size + padding),
                item_size,
                item_size};
            SDL_RenderDrawRect(renderer, &cell_rect);
        }
    }

    // Afficher les items
    for (int i = 0; i < inventory->item_count; ++i) {
        int row = i / inventory_size;
        int col = i % inventory_size;
        SDL_Rect item_rect = {
            start_x + col * (item_size + padding),
            start_y + row * (item_size + padding),
            item_size,
            item_size};
        SDL_RenderCopy(renderer, inventory->items[i].texture, NULL, &item_rect);
    }
}

void reset_game() {
    reset_game_state();
    reset_keys();
    player.life_points = 3;
    player.coins_count = 0;
    player_inventory.item_count = 0;
    quit_game = false;
    show_inventory = false;
    new_map = false;
    current_level = 1;
    current_map = 0;
    mario_gameType = true;
    pokemon_gameType = false;
}

void start_game(SDL_Renderer *renderer) {
    play_horn_sound();
    reset_game();
    // const char *klaxon = "assets/audio/klaxon.mp3";
    // play_sound(klaxon);
    current_level = 1;
    current_map = 0;
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

    Map *map = load_map();
    if (!map) {
        printf("Erreur lors du chargement de la carte\n");
        return;
    }

    player.texture = loadTexture("assets/sprites/naked-player.png", renderer);
    if (!player.texture) {
        printf("Erreur lors du chargement de la texture du joueur: %s\n", SDL_GetError());
        return;
    }

    surfer = loadTexture("assets/sprites/surfer-player.png", renderer);
    if (!surfer) {
        printf("Erreur lors du chargement de la texture du joueur: %s\n", SDL_GetError());
        return;
    }

    int cameraX = player.x - WINDOW_WIDTH / 2;
    int cameraY = player.y - WINDOW_HEIGHT / 2;

    while (!quit_game && !return_to_main_menu) {
        Uint32 currentTime = SDL_GetTicks();
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit_game = true;
                return_to_main_menu = true;
            } else if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
                handle_input(&event);
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE ) {
                    in_game_menu = !in_game_menu;
                }
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN && start_speech) {
                    if (speech_index == 2) {
                        if (player.coins_count >= 8) {
                            speech_index = 4;
                            player.coins_count -= 4;
                            surfboard = true;
                            surfsheet = true;
                        } else if (player.coins_count < 8) {
                            speech_index++;
                        }
                    } else if (speech_index == 3) {
                        start_speech = false;
                    } else {
                        speech_index++;
                        if (speech_index >= sizeof(dialogue) / sizeof(dialogue[0])) {
                            start_speech = false;
                        }
                    }
                }
            }
        }

        if (!in_game_menu) {
            if (surfsheet) {
                player.texture = surfer;
            }
            if (new_map) {
                blackout(renderer);
                reset_game_state();
                reset_keys();
                free_map(map);
                map = load_map();
                new_map = false;
                if (!map) {
                    printf("Erreur lors du chargement de la carte\n");
                    return;
                }
                cameraX = player.x - WINDOW_WIDTH / 2;
                cameraY = player.y - WINDOW_HEIGHT / 2;

                dim(renderer);
            }
            update_mobs(currentTime);
            animate_mobs(map->width, map->height, currentTime);
            update_player(renderer, map, map->width, map->height, currentTime);

            if (player.x > cameraX + WINDOW_WIDTH / 2 && cameraX + WINDOW_WIDTH < WORLD_WIDTH) {
                cameraX += SCROLL_SPEED;
            } else if (player.x < cameraX + WINDOW_WIDTH / 2 && cameraX > 0) {
                cameraX -= SCROLL_SPEED;
            }

            int targetCameraY = player.y - WINDOW_HEIGHT / 2;
            if (targetCameraY < 0) {
                cameraY = 0;
            } else if (targetCameraY + WINDOW_HEIGHT > WORLD_HEIGHT) {
                cameraY = WORLD_HEIGHT - WINDOW_HEIGHT;
            } else {
                cameraY = targetCameraY;
            }

            if (abs(cameraY - targetCameraY) < 1) {
                cameraY = targetCameraY;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            if (player.life_points > 0) {
                render_map(renderer, map, cameraX, cameraY);
                render_mobs(renderer, cameraX, cameraY);
                render_player(renderer, cameraX, cameraY);
                render_player_life(renderer, &player);
                render_coin_count(renderer, &player);
                if (show_inventory) {
                    render_inventory(renderer, &player_inventory);
                }
                if (start_speech) {
                    render_speech_bubble(renderer, dialogue[speech_index]);
                }
                if (surfboard && player_inventory.item_count < MAX_ITEMS) {
                    SDL_Texture *surf_texture = load_texture("assets/images/surf.png", renderer);
                    if (surf_texture) {
                        Item surf = {"Surfboard", surf_texture};
                        player_inventory.items[player_inventory.item_count] = surf;
                        player_inventory.item_count++;
                        surfboard = false;
                    }
                }
            } else if (player.life_points < 0) {
                play_win_sound();
                display_in_game_menu(renderer, &player, &return_to_main_menu, &resume_game);
            } else {
                player_inventory.item_count = 0;
                play_loss_sound();
                // play_sound("assets/audio/game-over.mp3");
                display_in_game_menu(renderer, &player, &return_to_main_menu, &resume_game);
            }
        } else {
            display_in_game_menu(renderer, &player, &return_to_main_menu, &resume_game);
            if (resume_game) {
                in_game_menu = false;
                resume_game = false;
            }
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 fps
    }

    free_map(map);
    free_block_textures();
}

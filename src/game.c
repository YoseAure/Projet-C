#include "../include/game.h"
#include "../include/menu.h"
#include "../include/settings.h"
#include "../include/maps.h"

extern bool exit_program;

Player player = {5 * TILE_SIZE,
                10 * TILE_SIZE, 
                TILE_SIZE, 
                TILE_SIZE * 2, 
                false, 
                0, 
                0,
                1,
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

void reset_player(Player *player) {
    player->x = 5 * TILE_SIZE;
    player->y = 10 * TILE_SIZE;
    player->life_points = 1; // Réinitialise la vie du joueur
    player->last_hit_time = 0;
    player->isJumping = false;
    player->y_speed = 0;
    // Réinitialiser l'inertie
    memset(keys, 0, sizeof(keys));
    player->coin_count = 0;
}

void reset_camera_position(int *cameraX) {
    *cameraX = 0; // Réinitialise la position de la caméra
}

void display_game_over_menu(SDL_Renderer *renderer) {
    // Réinitialise la vie du joueur et la position de la caméra au début de la fonction
    player.life_points = 1;
    int cameraX = 0;

    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 32);
    TTF_Font *font_large = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 36);
    TTF_Font *title_font = TTF_OpenFont("assets/fonts/mario-font-2.ttf", 72);

    if (!font || !title_font || !font_large) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char *options[] = {"Restart Game", "Main Menu", "Quit"};
    int selected = 0;
    bool quit = false;
    bool return_to_main_menu = false;
    SDL_Event event;

    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit_program = true;
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected - 1 + 3) % 3;
                        break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % 3;
                        break;
                    case SDLK_RETURN:
                        if (selected == 0) {
                            quit = true;
                            reset_player(&player); // Réinitialise la position et la vie du joueur
                            reset_camera_position(&cameraX); // Réinitialise la position de la caméra
                            start_game(renderer); // Recommence une nouvelle partie
                        } else if (selected == 1) {
                            return_to_main_menu = true;
                            quit = true;
                        } else if (selected == 2) {
                            exit_program = true;
                            quit = true;
                        }
                        break;
                    case SDLK_ESCAPE:
                        exit_program = true;
                        quit = true;player.life_points =0;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // titre : Game Over
        SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, "Game Over", white);
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

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }

    // Nettoyage
    TTF_CloseFont(font);
    TTF_CloseFont(font_large);
    TTF_CloseFont(title_font);

    if (return_to_main_menu) {
        reset_player(&player); 
        reset_camera_position(&cameraX);
        display_main_menu(renderer);
    }
}

int coin_count; // Compteur de pièces récupérées

void render_coin_count(SDL_Renderer *renderer, int coin_count) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    char coin_text[20];
    sprintf(coin_text, "Sexwax: %d", coin_count);

    SDL_Surface *surface = TTF_RenderText_Solid(font, coin_text, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    int text_width = surface->w;
    int text_height = surface->h;
    SDL_Rect dest = {WINDOW_WIDTH - text_width - 10, 10, text_width, text_height};

    // Dessiner un rectangle noir derrière le texte
    SDL_Rect background_rect = {dest.x - 5, dest.y - 5, text_width + 10, text_height + 10};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Couleur noire
    SDL_RenderFillRect(renderer, &background_rect);

    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void update_player(BlockType **map, int width, int height, Uint32 currentTime, SDL_Renderer *renderer) {
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
        }
    }

    if (keys[SDL_SCANCODE_SPACE] && !player.isJumping) {
        if (currentTime - player.lastJump_t >= JUMP_DELAY) {
            player.y_speed = JUMP_FORCE;
            player.isJumping = true;
            player.lastJump_t = currentTime;
        }
    }

    player.y += player.y_speed;
    if (player.isJumping) {
        player.y_speed += GRAVITY;
    }

    bool onGround = false;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            SDL_Rect obs = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            if (map[i][j] == GROUND || map[i][j] == BRICK) {
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
            } else if (map[i][j] == ENEMY) {
                if (checkCollision(&player, &obs)) {
                    if (currentTime - player.last_hit_time > 1000) { // 1 seconde d'invulnérabilité
                        player.life_points -= 1;
                        player.last_hit_time = currentTime;
                        printf("Player hit! Life points: %d\n", player.life_points);
                        if (player.life_points < 1) {
                            printf("Player died!\n");
                            player.coin_count = 0;
                            display_game_over_menu(renderer); // Affiche le menu de game over
                            return;
                        }
                    }
                }
            } else if (map[i][j] == COIN) {
                if (checkCollision(&player, &obs)) {
                    map[i][j] = EMPTY; // La pièce disparaît
                    player.coin_count++; // Incrémente le compteur de pièces
                    printf("Coin collected! Total coins: %d\n", player.coin_count);
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

    Map *map = load_map("assets/maps/map1.txt");
    if (!map) {
        printf("Erreur lors du chargement de la carte\n");
        return;
    }

    int cameraX = player.x - WINDOW_WIDTH / 2;
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
            update_player(map->blocks, map->width, map->height, currentTime, renderer);

            if (player.x > cameraX + WINDOW_WIDTH / 2 && cameraX + WINDOW_WIDTH < WORLD_WIDTH) {
                cameraX += SCROLL_SPEED;
            } else if (player.x < cameraX + WINDOW_WIDTH / 2 && cameraX > 0) {
                cameraX -= SCROLL_SPEED;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            if(player.life_points > 0) {
                render_map(renderer, map, cameraX);
                render_coin_count(renderer, player.coin_count); // Affiche le compteur de pièces
            } else {
                display_game_over_menu(renderer);
                render_map(renderer, map, WINDOW_WIDTH / 2);
            }
            
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
        reset_player(&player); // Réinitialise la vie du joueur
        reset_camera_position(&cameraX); // Réinitialise la position de la caméra
        display_main_menu(renderer);
    } else {
        free_map(map);
    }

    free_block_textures();
}

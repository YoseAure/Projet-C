#include "../include/game.h"
#include "../include/menu.h"

extern bool exit_program;

void display_in_game_menu(SDL_Renderer *renderer, bool *return_to_main_menu) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font.ttf", 24);
    if (!font) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char *options[] = {"Retourner au menu principal", "Options"};
    int selected = 0;
    bool quit = false;
    SDL_Event event;

    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                *return_to_main_menu = true;
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected - 1 + 2) % 2;
                        break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % 2;
                        break;
                    case SDLK_RETURN:
                        if (selected == 0) {
                            *return_to_main_menu = true;
                            quit = true;
                        } break;
                    case SDLK_ESCAPE:
                        quit = true;
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < 2; ++i) {
            SDL_Color color = (i == selected) ? yellow : white;

            SDL_Surface *surface = TTF_RenderText_Solid(font, options[i], color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            int text_width = surface->w;
            int text_height = surface->h;
            int x = (window_width - text_width) / 2;
            int y = (window_height / 2) + (i * (text_height + 10));

            SDL_Rect dest = {x, y, text_width, text_height};
            SDL_RenderCopy(renderer, texture, NULL, &dest);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
}

void start_game(SDL_Renderer *renderer) {
    bool quit_game = false;
    bool return_to_main_menu = false;
    bool in_game_menu = false;
    SDL_Event event;

    while (!quit_game && !return_to_main_menu) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit_game = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    in_game_menu = !in_game_menu;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (in_game_menu) {
            display_in_game_menu(renderer, &return_to_main_menu);
            in_game_menu = false;
        } else {
            // Logique du jeu
        }

        SDL_RenderPresent(renderer);
    }

    if (return_to_main_menu) {
        return_to_main_menu = false;
        display_main_menu(renderer);
    }
}

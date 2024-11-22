#include "../include/settings.h"


extern bool exit_program;

bool music_enabled = true;
bool sound_effects_enabled = true;

void settings(SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 32);
    TTF_Font *font_large = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 36);

    if (!font || !font_large) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char *options[] = {"turn music off", "turn sound effects", "back to main menu"};
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
                            music_enabled = !music_enabled;
                            if (!music_enabled) {
                                Mix_HaltMusic();
                            } else {
                                // Pour rejouer la musique si elle est reactivée
                                Mix_PlayMusic(Mix_LoadMUS("assets/audio/background-music-1.mp3"), -1);
                            }
                        } else if (selected == 1) {
                            sound_effects_enabled = !sound_effects_enabled;
                        } else if (selected == 2) {
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

        // Affichage des options du menu
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

    Mix_FreeChunk(menu_selection_sound);
    SDL_DestroyTexture(logo_texture);
    TTF_CloseFont(font);
    TTF_CloseFont(font_large);
}
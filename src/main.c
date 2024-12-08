#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "../include/menu.h"
#include "../include/game.h"
#include "../include/init.h"
#include "../include/cleanup.h"
#include "../include/tools.h"

bool exit_program = false;
extern bool quit_game;

typedef enum {
    MAIN_MENU,
    IN_GAME,
    // WIN_GAME,
    EXIT
} GameState;

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Mix_Music *background_music = NULL;

    if (!init_sdl(&window, &renderer, &background_music)) {
        return 1;
    }

    GameState game_state = MAIN_MENU;

    while (game_state != EXIT) {
        switch (game_state) {
            case MAIN_MENU:
                display_main_menu(renderer);
                if (exit_program) {
                    game_state = EXIT;
                } else {
                    game_state = IN_GAME;
                }
                break;
            case IN_GAME:
                start_game(renderer);
                if (exit_program) {
                    game_state = EXIT;
                } 
                // else if (game_won) {
                //     game_state = WIN_GAME;
                // } 
                else if (quit_game) {
                    game_state = MAIN_MENU;
                } else {
                    game_state = IN_GAME;
                }
                break;
    //         case WIN_GAME: {
    //             SDL_Surface *win_surface = SDL_LoadBMP("assets/images/win.png");
    //             if (!win_surface) {
    //                 printf("Erreur de chargement de l'image: %s\n", SDL_GetError());
    //                 game_state = MAIN_MENU;
    //                 break;
    //             }

    //             SDL_Texture *win_texture = SDL_CreateTextureFromSurface(renderer, win_surface);
    //             SDL_FreeSurface(win_surface);
    //             if (!win_texture) {
    //                 printf("Erreur de création de la texture: %s\n", SDL_GetError());
    //                 game_state = MAIN_MENU;
    //                 break;
    //             }

    //             bool stay_in_win = true;
    //             while (stay_in_win) {
    //                 SDL_RenderClear(renderer);

    //                 SDL_RenderCopy(renderer, win_texture, NULL, NULL);

    //                 SDL_RenderPresent(renderer);

    //                 SDL_Event event;
    //                 while (SDL_PollEvent(&event)) {
    //                     if (event.type == SDL_QUIT)
    //                     {
    //                         exit_program = true;
    //                         stay_in_win = false;
    //                         game_state = EXIT;
    //                     }
    //                     else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
    //                     {
    //                         stay_in_win = false;
    //                         game_state = MAIN_MENU;
    //                     }
    //                 }
    //                 SDL_Delay(16); // ~60 FPS
    //             }

    //             SDL_DestroyTexture(win_texture);
    //             break;
    //         }

            default:
                game_state = EXIT;
                break;
        }
    }

    cleanup_sdl(window, renderer, background_music);

    return 0;
}

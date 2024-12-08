#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "../include/menu.h"
#include "../include/game.h"
#include "../include/init.h"
#include "../include/cleanup.h"

bool exit_program = false;
extern bool quit_game;

typedef enum {
    MAIN_MENU,
    IN_GAME,
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
                } else if (quit_game) { // Ajoutez cette condition pour revenir au menu principal
                    game_state = MAIN_MENU;
                } else {
                    game_state = IN_GAME;
                }
                break;
            default:
                game_state = EXIT;
                break;
        }
    }

    cleanup_sdl(window, renderer, background_music);

    return 0;
}

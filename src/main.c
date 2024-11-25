#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "../include/menu.h"
#include "../include/game.h"
#include "../include/init.h"
#include "../include/cleanup.h"

bool exit_program = false;

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    Mix_Music *background_music = NULL;

    if (!init_sdl(&window, &renderer, &background_music)) {
        return 1;
    }

    while (!exit_program) {
        display_main_menu(renderer);
    }

    cleanup_sdl(window, renderer, background_music);

    return 0;
}

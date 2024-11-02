#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "../include/menu.h"
#include "../include/game.h"

bool exit_program = false;

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur Mix_OpenAudio: %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // musique de fond
    Mix_Music *background_music = Mix_LoadMUS("assets/audio/background-music-1.mp3");
    if (!background_music) {
        printf("Erreur Mix_LoadMUS: %s\n", Mix_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // musique de fond en boucle
    Mix_PlayMusic(background_music, -1);

    SDL_DisplayMode display_mode;
    if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0) {
        printf("Erreur SDL_GetCurrentDisplayMode: %s\n", SDL_GetError());
        Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    int screen_width = display_mode.w;
    int screen_height = display_mode.h;

    float reduction_factor = 0.9;
    int window_width = (int)(screen_width * reduction_factor);
    int window_height = (int)(screen_height * reduction_factor);

    SDL_Window *window = SDL_CreateWindow("Les aventures de Pestyflore", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        Mix_FreeMusic(background_music);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    while (!exit_program) {
        display_main_menu(renderer);
    }

    // Nettoyage
    Mix_HaltMusic();
    Mix_FreeMusic(background_music);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return 0;
}

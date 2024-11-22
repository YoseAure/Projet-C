#include "../include/init.h"

bool init_sdl(SDL_Window **window, SDL_Renderer **renderer, Mix_Music **background_music) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1) {
        printf("Erreur TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("Erreur Mix_OpenAudio: %s\n", Mix_GetError());
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    *background_music = Mix_LoadMUS("assets/audio/background-music-1.mp3");
    if (!*background_music) {
        printf("Erreur Mix_LoadMUS: %s\n", Mix_GetError());
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    Mix_PlayMusic(*background_music, -1);

    SDL_DisplayMode display_mode;
    if (SDL_GetCurrentDisplayMode(0, &display_mode) != 0) {
        printf("Erreur SDL_GetCurrentDisplayMode: %s\n", SDL_GetError());
        Mix_FreeMusic(*background_music);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    // int screen_width = display_mode.w;
    // int screen_height = display_mode.h;

    // // float reduction_factor = 0.9;
    // int window_width = (int)(screen_width * reduction_factor);
    // int window_height = (int)(screen_height * reduction_factor);

    // int window_width = 1600;
    // int window_height = 900;

    *window = SDL_CreateWindow("Les aventures de Pestyflore", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        Mix_FreeMusic(*background_music);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        Mix_FreeMusic(*background_music);
        Mix_CloseAudio();
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

#include "../include/cleanup.h"

void cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer, Mix_Music *background_music) {
    Mix_HaltMusic();
    Mix_FreeMusic(background_music);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}
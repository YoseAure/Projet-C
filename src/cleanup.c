#include "../include/cleanup.h"

void cleanup_sdl(SDL_Window *window, SDL_Renderer *renderer) {
    Mix_HaltMusic();
    freeAudioAssets();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}
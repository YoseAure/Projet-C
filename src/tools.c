#include "../include/tools.h"

void render_logo(SDL_Renderer *renderer, SDL_Texture *logo_texture, int option_y, int option_x) {
    int logo_width = 50;
    int logo_height = 50;
    SDL_Rect logo_dest = {option_x - logo_width - 5, option_y + (logo_height / 4) - 15, logo_width, logo_height};
    SDL_RenderCopy(renderer, logo_texture, NULL, &logo_dest);
}

void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// void play_sound(const char *sound_path) {
//     Mix_Chunk *sound = Mix_LoadWAV(sound_path);
//     if (!sound) {
//         printf("Erreur Mix_LoadWAV: %s\n", Mix_GetError());
//         return;
//     }
//     Mix_PlayChannel(-1, sound, 0);
//     Mix_FreeChunk(sound);
// }

// pas propre ...
void play_life_loss_sound() {
    Mix_Chunk *life_loss = Mix_LoadWAV("assets/audio/hp-loss.mp3");
    if (life_loss == NULL) {
        printf("Failed to load life loss sound: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, life_loss, 0);
}

void play_horn_sound() {
    Mix_Chunk *horn = Mix_LoadWAV("assets/audio/klaxon.mp3");
    if (horn == NULL) {
        printf("Failed to load horn sound: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, horn, 0);
}

void play_loss_sound() {
    Mix_Chunk *loss = Mix_LoadWAV("assets/audio/game-over.mp3");
    if (loss == NULL) {
        printf("Failed to load loss sound: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, loss, 0);
}

void play_pop_sound() {
    Mix_Chunk *pop = Mix_LoadWAV("assets/audio/pop-sound.mp3");
    if (pop == NULL) {
        printf("Failed to load pop sound: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, pop, 0);
}

void play_win_sound() {
    Mix_Chunk *win = Mix_LoadWAV("assets/audio/win.mp3");
    if (win == NULL) {
        printf("Failed to load win sound: %s\n", Mix_GetError());
        return;
    }
    Mix_PlayChannel(-1, win, 0);
}
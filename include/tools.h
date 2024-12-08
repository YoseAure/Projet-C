#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void render_logo(SDL_Renderer *renderer, SDL_Texture *logo_texture, int option_y, int option_x);
void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y);
void play_sound (const char *sound_path);
void play_horn_sound();
void play_loss_sound();
void play_life_loss_sound();
void play_pop_sound();
void play_win_sound();

#endif // TOOLS_H
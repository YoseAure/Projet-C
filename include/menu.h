#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

typedef struct {
    int x, y;
} Position;

void display_main_menu(SDL_Renderer *renderer);
void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y);
void render_logo(SDL_Renderer *renderer, SDL_Texture *logo_texture, int option_y, int option_x);

#endif

#ifndef MENU_H
#define MENU_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct {
    int x, y;
} Position;

void display_main_menu(SDL_Renderer *renderer);
void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y);

#endif

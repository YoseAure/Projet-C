#ifndef TOOLS_H
#define TOOLS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

void render_logo(SDL_Renderer *renderer, SDL_Texture *logo_texture, int option_y, int option_x);
void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y);

#endif // TOOLS_H
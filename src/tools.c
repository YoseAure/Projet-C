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
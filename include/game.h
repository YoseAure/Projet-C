#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

void start_game(SDL_Renderer *renderer);
void display_in_game_menu(SDL_Renderer *renderer, bool *return_to_main_menu);

#endif // GAME_H
#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

typedef struct {
    int x, y, width, height;
    bool isJumping;
    int y_speed;
    Uint32 lastJump_t;
    int life_points;
    Uint32 last_hit_time; // Ajoutez ce champ
} Player;

void start_game(SDL_Renderer *renderer);
void display_in_game_menu(SDL_Renderer *renderer, bool *return_to_main_menu, bool *resume_game);

#endif // GAME_H
#ifndef GAME_H
#define GAME_H



#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdbool.h>
#include "constants.h"
#include "structs.h"
#include "sprite.h"
#include "settings.h"
#include "maps.h"
#include "tools.h"
#include "sounds.h"

void render_inventory(SDL_Renderer *renderer, Inventory *inventory);
void start_game(SDL_Renderer *renderer);
void display_in_game_menu(SDL_Renderer *renderer, Player *player, bool *return_to_main_menu, bool *resume_game);
void render_player(SDL_Renderer *renderer, int cameraX, int cameraY);
void render_mobs(SDL_Renderer *renderer, int cameraX, int cameraY);
void animate_mobs(int width, int height, Uint32 currentTime);
void update_mobs(Map *map, int width, int height, Uint32 currentTime);
void update_player(SDL_Renderer *renderer, Map *map, int width, int height, Uint32 currentTime);
void reset_game_state();

#endif // GAME_H
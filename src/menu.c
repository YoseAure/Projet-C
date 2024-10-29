#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "menu.h"

// Structure pour stocker les informations des options de menu
typedef struct
{
    const char *text;
    SDL_Color color;
    SDL_Rect rect;
} MenuOption;

void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void display_main_menu(SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font.ttf", 24);

    if (!font)
    {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    const char *options[] = {"Start Game", "Options", "Quit"};
    int selected = 0;
    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    selected = (selected - 1 + 3) % 3;
                    break;
                case SDLK_DOWN:
                    selected = (selected + 1) % 3;
                    break;
                case SDLK_RETURN:
                    if (selected == 2)
                        quit = true;
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Noir de fond
        SDL_RenderClear(renderer);

        // Affichage du menu
        for (int i = 0; i < 3; i++)
        {
            SDL_Color color = (i == selected) ? yellow : white;
            render_text(renderer, options[i], font, color, 280, 150 + i * 40);
        }

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
}

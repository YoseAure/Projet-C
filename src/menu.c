#include "../include/menu.h"
#include "../include/game.h"

// Structure pour stocker les informations des options de menu
typedef struct {
    const char *text;
    SDL_Color color;
    SDL_Rect rect;
} MenuOption;

void render_text(SDL_Renderer *renderer, const char *text, TTF_Font *font, SDL_Color color, int x, int y) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render_logo(SDL_Renderer *renderer, SDL_Texture *logo_texture, int option_y, int option_x) {
    int logo_width = 50;
    int logo_height = 50;

    SDL_Rect logo_dest = {option_x - logo_width - 5, option_y + (logo_height / 4) - 20, logo_width, logo_height};

    SDL_RenderCopy(renderer, logo_texture, NULL, &logo_dest);
}

void display_main_menu(SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font.ttf", 24);
    TTF_Font *font_large = TTF_OpenFont("assets/fonts/mario-font.ttf", 28);
    TTF_Font *title_font = TTF_OpenFont("assets/fonts/mario-font.ttf", 48);
    TTF_Font *title_font_large = TTF_OpenFont("assets/fonts/mario-font.ttf", 54);

    if (!font || !font_large || !title_font || !title_font_large) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Color white = {255, 255, 255, 255};
    SDL_Color yellow = {255, 255, 0, 255};

    SDL_Surface *logo_surface = IMG_Load("assets/images/surf.png");
    if (!logo_surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *logo_texture = SDL_CreateTextureFromSurface(renderer, logo_surface);
    SDL_FreeSurface(logo_surface);

    const char *title = "PestyVentura";
    const char *options[] = {"Start Game", "Options", "Quit"};
    int selected = 0;
    bool quit = false;
    SDL_Event event;

    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    Uint32 start_time = SDL_GetTicks();
    int blink_interval = 1500; // Intervalle de clignotement en millisecondes

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected - 1 + 3) % 3;
                        break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % 3;
                        break;
                    case SDLK_RETURN:
                        if (selected == 0) {
                            start_game(renderer);
                        } else if (selected == 2) {
                            quit = true;
                        }
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Déterminer la couleur et la taille de la police du titre en fonction du temps écoulé
        Uint32 current_time = SDL_GetTicks();
        bool is_yellow = ((current_time - start_time) / blink_interval) % 2 == 0;
        SDL_Color title_color = is_yellow ? yellow : white;
        TTF_Font *current_title_font = is_yellow ? title_font_large : title_font;

        // Afficher le titre
        SDL_Surface *title_surface = TTF_RenderText_Solid(current_title_font, title, title_color);
        SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
        int title_width = title_surface->w;
        int title_height = title_surface->h;
        int title_x = (window_width - title_width) / 2;
        int title_y = window_height / 4;
        SDL_Rect title_dest = {title_x, title_y, title_width, title_height};
        SDL_RenderCopy(renderer, title_texture, NULL, &title_dest);
        SDL_FreeSurface(title_surface);
        SDL_DestroyTexture(title_texture);

        // Afficher les options du menu et le logo à côté de l'option sélectionnée
        for (int i = 0; i < 3; ++i) {
            SDL_Color color = (i == selected) ? yellow : white;
            TTF_Font *current_font = (i == selected) ? font_large : font;

            SDL_Surface *surface = TTF_RenderText_Solid(current_font, options[i], color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            int text_width = surface->w;
            int text_height = surface->h;
            int x = (window_width - text_width) / 2;
            int y = (window_height / 2) + (i * (text_height + 20));

            SDL_Rect dest = {x, y, text_width, text_height};
            SDL_RenderCopy(renderer, texture, NULL, &dest);

            // Afficher le logo juste avant l'option sélectionnée
            if (i == selected) {
                render_logo(renderer, logo_texture, y, x);
            }

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(logo_texture);
    TTF_CloseFont(font);
    TTF_CloseFont(font_large);
    TTF_CloseFont(title_font);
    TTF_CloseFont(title_font_large);
}

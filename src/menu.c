#include "../include/menu.h"
#include "../include/game.h"

extern bool exit_program;

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

// Fonction pour afficher le logo à côté d'une option de menu
void render_logo(SDL_Renderer *renderer, SDL_Texture *logo_texture, int option_y, int option_x) {
    int logo_width = 50;
    int logo_height = 50;
    SDL_Rect logo_dest = {option_x - logo_width - 5, option_y + (logo_height / 4) - 20, logo_width, logo_height};
    SDL_RenderCopy(renderer, logo_texture, NULL, &logo_dest);
}

// Fonction d'animation d'introduction : le van se déplace de gauche à droite, le texte de droite à gauche
void play_intro_animation(SDL_Renderer *renderer, SDL_Texture *van_texture, const char *title, TTF_Font *title_font, TTF_Font *font, const char **options, int options_count, int *final_van_x, int *final_van_y) {
    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    int van_x = -200;
    int van_y = window_height - (window_height / 4);
    int van_width = 150;
    int van_height = 100;
    int van_speed_x = 9;
    int van_wave_amplitude = 4;

    int text_x = window_width; // Le texte démarre de la droite de l'écran
    int title_y = window_height / 4;

    SDL_Event event;
    bool skip_animation = false;
    Uint32 start_time = SDL_GetTicks();

    // Calcul de la position de départ du groupe d'options
    int options_spacing = 20;
    SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, title, (SDL_Color){255, 255, 0, 255});
    int title_height = title_surface->h;
    SDL_FreeSurface(title_surface);
    int options_group_height = (options_count * (title_height + options_spacing)) - options_spacing;
    int options_start_y = window_height / 2;

    while (text_x > window_width / 2 && !skip_animation) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                skip_animation = true;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                skip_animation = true;
            }
        }

        // Déplacement du van
        van_x += van_speed_x;
        van_y = window_height - (window_height / 4) + (van_wave_amplitude * sin(0.05 * van_x));

        *final_van_x = van_x;
        *final_van_y = van_y;

        // Déplacement du texte vers la gauche (opposé au van)
        text_x -= 7;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Affichage du van
        SDL_Rect van_dest = {van_x, van_y, van_width, van_height};
        SDL_RenderCopy(renderer, van_texture, NULL, &van_dest);

        // Affichage du titre
        SDL_Color yellow = {255, 255, 0, 255};
        SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, title, yellow);
        SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
        int title_width = title_surface->w;
        int title_height = title_surface->h;
        SDL_Rect title_dest = {text_x - title_width / 2, title_y, title_width, title_height};
        SDL_RenderCopy(renderer, title_texture, NULL, &title_dest);
        SDL_FreeSurface(title_surface);
        SDL_DestroyTexture(title_texture);

        // Affichage des options de menu centrées et alignées avec le titre
        for (int i = 0; i < options_count; ++i) {
            SDL_Color color = {255, 255, 255, 255};
            SDL_Surface *surface = TTF_RenderText_Solid(font, options[i], color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            int text_width = surface->w;
            int text_height = surface->h;

            // Calcul de la position centrée pour chaque option
            int x = text_x - text_width / 2;
            int y = options_start_y + (i * (text_height + options_spacing));

            SDL_Rect dest = {x, y, text_width, text_height};
            SDL_RenderCopy(renderer, texture, NULL, &dest);

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(30);
    }

    // Ajustement de la position du texte si l'utilisateur passe l'animation
    if (skip_animation) {
        text_x = (window_width - 300) / 2;
    }
}

// Fonction principale du menu
void display_main_menu(SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font.ttf", 24);
    TTF_Font *font_large = TTF_OpenFont("assets/fonts/mario-font.ttf", 28);
    TTF_Font *title_font = TTF_OpenFont("assets/fonts/mario-font.ttf", 48);
    TTF_Font *title_font_large = TTF_OpenFont("assets/fonts/mario-font.ttf", 54);

    if (!font || !font_large || !title_font || !title_font_large) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Surface *van_surface = IMG_Load("assets/images/van.png");
    if (!van_surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *van_texture = SDL_CreateTextureFromSurface(renderer, van_surface);
    SDL_FreeSurface(van_surface);

    SDL_Surface *logo_surface = IMG_Load("assets/images/surf.png");
    if (!logo_surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *logo_texture = SDL_CreateTextureFromSurface(renderer, logo_surface);
    SDL_FreeSurface(logo_surface);

    int final_van_x = 0;
    int final_van_y = 0;

    const char *title = "PestyVentura";
    const char *options[] = {"Start Game", "Options", "Quit"};
    int options_count = sizeof(options) / sizeof(options[0]);
    play_intro_animation(renderer, van_texture, title, title_font, font, options, options_count, &final_van_x, &final_van_y);
    int selected = 0;
    bool quit = false;
    SDL_Event event;

    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    Uint32 start_time = SDL_GetTicks();
    int blink_interval = 1500;

    while (!quit && !exit_program) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit_program = true;
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
                            exit_program = true;
                            quit = true;
                        } break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Affichage du titre qui clignote
        Uint32 current_time = SDL_GetTicks();
        bool is_yellow = ((current_time - start_time) / blink_interval) % 2 == 0;
        SDL_Color title_color = is_yellow ? (SDL_Color){255, 255, 0, 255} : (SDL_Color){255, 255, 255, 255};
        TTF_Font *current_title_font = is_yellow ? title_font_large : title_font;

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

        // Affichage des options du menu
        for (int i = 0; i < 3; ++i) {
            SDL_Color color = (i == selected) ? (SDL_Color){255, 255, 0, 255} : (SDL_Color){255, 255, 255, 255};
            TTF_Font *current_font = (i == selected) ? font_large : font;

            SDL_Surface *surface = TTF_RenderText_Solid(current_font, options[i], color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            int text_width = surface->w;
            int text_height = surface->h;
            int x = (window_width - text_width) / 2;
            int y = (window_height / 2) + (i * (text_height + 20));

            SDL_Rect dest = {x, y, text_width, text_height};
            SDL_RenderCopy(renderer, texture, NULL, &dest);

            if (i == selected) {
                render_logo(renderer, logo_texture, y, x);
            }

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        int van_width = 150;
        int van_height = 100;
        SDL_Rect final_van_dest = {final_van_x, final_van_y, van_width, van_height};
        SDL_RenderCopy(renderer, van_texture, NULL, &final_van_dest);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(van_texture);
    SDL_DestroyTexture(logo_texture);
    TTF_CloseFont(font);
    TTF_CloseFont(font_large);
    TTF_CloseFont(title_font);
    TTF_CloseFont(title_font_large);
}
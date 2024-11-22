#include "../include/menu.h"
#include "../include/game.h"
#include "../include/settings.h"
#include "../include/tools.h"
#include "../include/maps.h"

extern bool exit_program;

// Structure pour stocker les informations des options de menu
typedef struct {
    const char *text;
    SDL_Color color;
    SDL_Rect rect;
} MenuOption;


void play_intro_animation(SDL_Renderer *renderer,
                          SDL_Texture *van_texture,
                          const char *title,
                          TTF_Font *title_font,
                          TTF_Font *font,
                          const char **options,
                          int options_count,
                          int *final_van_x,
                          int *final_van_y)
{
    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    int van_x = -200;
    int van_y = window_height - (window_height / 4);
    int van_width = 300;
    int van_height = 200;
    int van_speed_x = 9;
    int van_wave_amplitude = 4;

    int text_x = window_width; // Le texte démarre de la droite de l'écran
    int title_y = window_height / 4;

    int background_x = - window_width / 2;
    int background_speed_x = 7;

    SDL_Surface *background_surface = IMG_Load("assets/images/plage.jpg");
    if (!background_surface) {
        printf("Erreur IMG_Load: %s\n", IMG_GetError());
        return;
    }
    SDL_Texture *background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
    SDL_FreeSurface(background_surface);

    SDL_Event event;
    bool skip_animation = false;

    // Calcul de la position de départ du groupe d'options
    int options_spacing = 20;
    SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, title, (SDL_Color){0, 0, 0, 255});
    int title_height = title_surface->h;
    SDL_FreeSurface(title_surface);
    int options_group_height = (options_count * (title_height + options_spacing)) - options_spacing;
    int options_start_y = window_height / 2;

    while (text_x > window_width / 2 && !skip_animation) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                skip_animation = true;
            } else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                skip_animation = true;
            }
        }

        // Déplacement du van
        van_x += van_speed_x;
        van_y = window_height - (window_height / 4) + (van_wave_amplitude * sin(0.05 * van_x));

        *final_van_x = van_x;
        *final_van_y = van_y;

        // Déplacement du texte et de l'image de fond vers la gauche
        text_x -= background_speed_x;
        background_x -= background_speed_x;

        // Réinitialiser la position de l'image de fond pour créer un effet de boucle
        if (background_x <= -window_width) {
            background_x = 0;
        }

        // Affichage de l'image de fond en défilement
        SDL_Rect background_dest1 = {background_x, 0, window_width, window_height};
        SDL_Rect background_dest2 = {background_x + window_width, 0, window_width, window_height};
        SDL_RenderCopy(renderer, background_texture, NULL, &background_dest1);
        SDL_RenderCopy(renderer, background_texture, NULL, &background_dest2);

        // Affichage du van
        SDL_Rect van_dest = {van_x, van_y, van_width, van_height};
        SDL_RenderCopy(renderer, van_texture, NULL, &van_dest);

        // Affichage du titre
        SDL_Color noir = {0, 0, 0, 255};
        SDL_Surface *title_surface = TTF_RenderText_Solid(title_font, title, noir);
        SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
        int title_width = title_surface->w;
        int title_height = title_surface->h;
        SDL_Rect title_dest = {text_x - title_width / 2, title_y, title_width, title_height};
        SDL_RenderCopy(renderer, title_texture, NULL, &title_dest);
        SDL_FreeSurface(title_surface);
        SDL_DestroyTexture(title_texture);

        // Affichage des options de menu centrées et alignées avec le titre
        for (int i = 0; i < options_count; ++i) {
            SDL_Color color = {0, 0, 0, 255}; // Noir
            SDL_Surface *surface = TTF_RenderText_Solid(font, options[i], color);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            int text_width = surface->w;
            int text_height = surface->h;

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

    SDL_DestroyTexture(background_texture);

    if (skip_animation) {
        text_x = (window_width - 300) / 2;
    }
}

// Fonction principale du menu
void display_main_menu(SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 32);
    TTF_Font *font_large = TTF_OpenFont("assets/fonts/mario-font-pleine.ttf", 36);
    TTF_Font *title_font = TTF_OpenFont("assets/fonts/mario-font-2.ttf", 72);
    TTF_Font *title_font_large = TTF_OpenFont("assets/fonts/mario-font-2.ttf", 78);

    if (!font || !font_large || !title_font || !title_font_large) {
        printf("Erreur TTF_OpenFont: %s\n", TTF_GetError());
        return;
    }

    SDL_Surface *van_surface = IMG_Load("assets/images/pestyCar.png");
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
    const char *options[] = {"Start Game", "Settings", "Quit"};
    int options_count = sizeof(options) / sizeof(options[0]);
    play_intro_animation(renderer, van_texture, title, title_font, font, options, options_count, &final_van_x, &final_van_y);
    int selected = 0;
    bool quit = false;
    SDL_Event event;

    int window_width, window_height;
    SDL_GetRendererOutputSize(renderer, &window_width, &window_height);

    Uint32 start_time = SDL_GetTicks();
    int blink_interval = 1500;

    Mix_Chunk *menu_selection_sound = Mix_LoadWAV("assets/audio/pop-sound.mp3");
    if (!menu_selection_sound) {
        printf("Erreur Mix_LoadWAV: %s\n", Mix_GetError());
        exit_program = true;
        quit = true;
    }

    while (!quit && !exit_program) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                exit_program = true;
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        selected = (selected - 1 + 3) % 3;
                        if (sound_effects_enabled) {
                            Mix_PlayChannel(-1, menu_selection_sound, 0);
                        } break;
                    case SDLK_DOWN:
                        selected = (selected + 1) % 3;
                        if (sound_effects_enabled) {
                            Mix_PlayChannel(-1, menu_selection_sound, 0);
                        } break;
                    case SDLK_RETURN:
                        if (selected == 0) {
                            start_game(renderer);
                            quit = true;
                        } else if (selected == 1) {
                            settings(renderer);
                        } else if (selected == 2) {
                            exit_program = true;
                            quit = true;
                        } break;
                }
            }
        }

        SDL_Surface *background_surface = IMG_Load("assets/images/plage.jpg");
        if (!background_surface) {
            printf("Erreur IMG_Load: %s\n", IMG_GetError());
            exit_program = true;
            quit = true;
        }
        SDL_Texture *background_texture = SDL_CreateTextureFromSurface(renderer, background_surface);
        SDL_FreeSurface(background_surface);

        SDL_RenderCopy(renderer, background_texture, NULL, NULL);
        SDL_DestroyTexture(background_texture);

        // Affichage du titre qui clignote
        Uint32 current_time = SDL_GetTicks();
        bool is_orange = ((current_time - start_time) / blink_interval) % 2 == 0;
        SDL_Color title_color = is_orange ? (SDL_Color){0, 0, 0, 255} : (SDL_Color){0, 0, 0, 255}; // Noir
        TTF_Font *current_title_font = is_orange ? title_font_large : title_font;

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

        const char *subtitle = "L'apprentie surfeuse";
        SDL_Color subtitle_color = {255, 0, 0, 255}; // Rouge

        SDL_Surface *subtitle_surface = TTF_RenderText_Solid(font, subtitle, subtitle_color);
        SDL_Texture *subtitle_texture = SDL_CreateTextureFromSurface(renderer, subtitle_surface);
        int subtitle_width = subtitle_surface->w;
        int subtitle_height = subtitle_surface->h;

        int subtitle_x = title_x + title_width - (subtitle_width / 2);
        int subtitle_y = title_y + title_height - (subtitle_height / 2);

        // Rotation
        double angle = -15.0;                                                   // Angle de rotation en degrés
        SDL_Point rotation_center = {subtitle_width / 2, subtitle_height / 2}; // Rotation autour du centre du texte

        SDL_Rect subtitle_dest = {subtitle_x, subtitle_y, subtitle_width, subtitle_height};
        SDL_RenderCopyEx(renderer, subtitle_texture, NULL, &subtitle_dest, angle, &rotation_center, SDL_FLIP_NONE);

        SDL_FreeSurface(subtitle_surface);
        SDL_DestroyTexture(subtitle_texture);

        // Affichage des options du menu
        for (int i = 0; i < 3; ++i) {
            SDL_Color color = (i == selected) ? (SDL_Color){255, 255, 0, 255} : (SDL_Color){0, 0, 0, 255}; 
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

        int van_width = 300;
        int van_height = 200;
        SDL_Rect final_van_dest = {final_van_x, final_van_y, van_width, van_height};
        SDL_RenderCopy(renderer, van_texture, NULL, &final_van_dest);

        SDL_RenderPresent(renderer);
    }

    Mix_FreeChunk(menu_selection_sound);
    SDL_DestroyTexture(van_texture);
    SDL_DestroyTexture(logo_texture);
    TTF_CloseFont(font);
    TTF_CloseFont(font_large);
    TTF_CloseFont(title_font);
    TTF_CloseFont(title_font_large);
}
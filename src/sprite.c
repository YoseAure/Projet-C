#include "sprite.h"

SDL_Texture *loadTexture(const char *path, SDL_Renderer *renderer) {
    SDL_Surface *tempSurface = IMG_Load(path);
    if (!tempSurface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}

void getSpriteClips(SDL_Rect clips[MAX_SPRITE_ROWS][MAX_SPRITE_COLS], int rows, int cols, int sprite_width, int sprite_height) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            clips[i][j].x = j * sprite_width;
            clips[i][j].y = i * sprite_height;
            clips[i][j].w = sprite_width;
            clips[i][j].h = sprite_height;
        }
    }
}
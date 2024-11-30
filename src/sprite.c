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

void getSpriteClips(SDL_Rect clips[SPRITE_ROWS][SPRITE_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            clips[i][j].x = j * SPRITE_WIDTH;
            clips[i][j].y = i * SPRITE_HEIGHT;
            clips[i][j].w = SPRITE_WIDTH;
            clips[i][j].h = SPRITE_HEIGHT;
        }
    }
}
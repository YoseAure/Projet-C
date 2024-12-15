#include <sounds.h>

bool music_enabled = true;
bool sound_effects_enabled = true;


int loadAudioAssets() {
    printf("Chargement des sons...\n");
    soundEffects[DAMAGE_SOUND] = Mix_LoadWAV("assets/audio/damage.mp3");
    soundEffects[MENU_SELECTION_SOUND] = Mix_LoadWAV("assets/audio/menu-selection.mp3");
    soundEffects[POP_SOUND] = Mix_LoadWAV("assets/audio/pop-sound.mp3");
    soundEffects[KLAXON_SOUND] = Mix_LoadWAV("assets/audio/klaxon.mp3");
    soundEffects[GAME_OVER_SOUND] = Mix_LoadWAV("assets/audio/game-over.mp3");
    soundEffects[WIN_SOUND] = Mix_LoadWAV("assets/audio/win.mp3");

    for (int i = 0; i < NUM_SOUNDS; i++) {
        if (soundEffects[i] == NULL) {
            fprintf(stderr, "Erreur de chargement du son %d: %s\n", i, Mix_GetError());
            return -1;
        }
    }

    printf("\033[0;32mChargement des sons terminé.\033[0m\n");
    printf("Chargement des musiques...\n");

    musicTracks[BACKGROUND_MUSIC_1] = Mix_LoadMUS("assets/audio/background-music-1.mp3");
    musicTracks[BACKGROUND_MUSIC_2] = Mix_LoadMUS("assets/audio/background-music-2.mp3");
    musicTracks[SURFSHOP_MUSIC] = Mix_LoadMUS("assets/audio/surfshop.mp3");

    for (int i = 0; i < NUM_MUSIC; i++) {
        if (musicTracks[i] == NULL) {
            fprintf(stderr, "Erreur de chargement de la musique %d: %s\n", i, Mix_GetError());
            return -1;
        }
    }
    printf("\033[0;32mChargement des musiques terminé.\033[0m\n");
    return 0;
}

void playSoundEffect(SoundEffectIndex index) {
    if (!sound_effects_enabled) {
        return;
    }
    if (index < 0 || index >= NUM_SOUNDS) {
        fprintf(stderr, "Index du son invalide : %d\n", index);
        return;
    }

    if (Mix_PlayChannel(-1, soundEffects[index], 0) == -1) {
        fprintf(stderr, "Erreur de lecture du son: %s\n", Mix_GetError());
    }
}

void playMusic(MusicIndex index) {
    if (!music_enabled) {
        return;
    }
    if (index < 0 || index >= NUM_MUSIC) {
        fprintf(stderr, "\033[0;31mIndex de la musique invalide : %d\033[0m\n", index);
        return;
    }

    if (Mix_PlayMusic(musicTracks[index], -1) == -1) {
        fprintf(stderr, "Erreur de lecture de la musique: %s\n", Mix_GetError());
    }
}

// void stopMusic() {
//     Mix_HaltMusic();
// }

void pauseOrResumeMusic() {
    if (Mix_PausedMusic()) {
        Mix_ResumeMusic();
    } else {
        Mix_PauseMusic();
    }
}

void changeMusic(MusicIndex index) {
    if (!music_enabled) {
        return;
    }
    if (index < 0 || index >= NUM_MUSIC) {
        fprintf(stderr, "\033[0;31mIndex de la musique invalide : %d\033[0m\n", index);
        return;
    }

    Mix_HaltMusic();
    playMusic(index);
}

void freeAudioAssets() {
    for (int i = 0; i < NUM_SOUNDS; i++) {
        if (soundEffects[i]) {
            Mix_FreeChunk(soundEffects[i]);
        }
    }

    for (int i = 0; i < NUM_MUSIC; i++) {
        if (musicTracks[i]) {
            Mix_FreeMusic(musicTracks[i]);
        }
    }
    printf("Libération des ressources audio terminée.\n");
}

// int loadSounds() {
//     sounds[BACKGROUND_MUSIC_1] = Mix_LoadWAV("assets/audio/background-music-1.mp3");
//     sounds[BACKGROUND_MUSIC_2] = Mix_LoadWAV("assets/audio/background-music-2.mp3");
//     sounds[SURFSHOP_MUSIC] = Mix_LoadWAV("assets/audio/surfshop.mp3");
//     sounds[KLAXON_SOUND] = Mix_LoadWAV("assets/audio/klaxon.mp3");
//     sounds[MENU_SELECTION_SOUND] = Mix_LoadWAV("assets/audio/menu-selection.mp3");
//     sounds[POP_SOUND] = Mix_LoadWAV("assets/audio/pop-sound.mp3");
//     sounds[DAMAGE_SOUND] = Mix_LoadWAV("assets/audio/damage.mp3");
//     sounds[GAME_OVER_SOUND] = Mix_LoadWAV("assets/audio/game-over.mp3");
//     sounds[WIN_SOUND] = Mix_LoadWAV("assets/audio/win.mp3");

//     for (int i = 0; i < NUM_SOUNDS; i++) {
//         if (sounds[i] == NULL) {
//             fprintf(stderr, "Erreur de chargement du son %d: %s\n", i, Mix_GetError());
//             return -1;
//         }
//     }

//     return 0;
// }

// void playSound(SoundIndex soundIndex) {
//     if (soundIndex < 0 || soundIndex >= NUM_SOUNDS) {
//         fprintf(stderr, "Index du son invalide : %d\n", soundIndex);
//         return;
//     }

//     if (Mix_PlayChannel(-1, sounds[soundIndex], 0) == -1) {
//         fprintf(stderr, "Erreur de lecture du son: %s\n", Mix_GetError());
//     }
// }

// void stopSound(SoundIndex soundIndex) {
//     if (soundIndex < 0 || soundIndex >= NUM_SOUNDS && soundIndex != ALL_SOUNDS) {
//         fprintf(stderr, "Index du son invalide : %d\n", soundIndex);
//         return;
//     }

//     if (soundIndex != ALL_SOUNDS) {
//         Mix_HaltChannel(soundIndex);
//     } else {
//         Mix_HaltChannel(-1);
//     }
// }

// void freeSounds() {
//     for (int i = 0; i < NUM_SOUNDS; i++) {
//         if (sounds[i]) {
//             Mix_FreeChunk(sounds[i]);
//         }
//     }
// }
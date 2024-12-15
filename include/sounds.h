#ifndef SOUNDS_H
#define SOUNDS_H

#include <SDL2/SDL_mixer.h>
#include <stdbool.h>

typedef enum {
    DAMAGE_SOUND,
    MENU_SELECTION_SOUND,
    POP_SOUND,
    KLAXON_SOUND,
    GAME_OVER_SOUND,
    WIN_SOUND,
    NUM_SOUNDS
} SoundEffectIndex;

typedef enum {
    BACKGROUND_MUSIC_1,
    BACKGROUND_MUSIC_2,
    SURFSHOP_MUSIC,
    NUM_MUSIC
} MusicIndex;

Mix_Chunk *soundEffects[NUM_SOUNDS];
Mix_Music *musicTracks[NUM_MUSIC];

extern bool music_enabled;
extern bool sound_effects_enabled;

// Function declarations for sound.c
// void initializeSoundSystem();
// void loadSound(const char* filename);
// void playSound(int soundID);
// void stopSound(int soundID);
// void cleanupSoundSystem();

int loadAudioAssets();
void playSoundEffect(SoundEffectIndex index);
void playMusic(MusicIndex index);
void pauseOrResumeMusic();
void changeMusic(MusicIndex index);
// void stopMusic();
void freeAudioAssets();
// int loadSounds();
// void playSound(SoundIndex soundIndex);
// void stopSound(SoundIndex soundIndex);
// void freeSounds();

#endif // SOUNDS_H
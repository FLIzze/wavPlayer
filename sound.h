#ifndef SOUND_H
#define SOUND_H

#include <string>
#include <irrKlang.h>

bool initSoundEngine();
bool playMusic();
void pauseMusic();
void resumeMusic();
void cleanupSoundEngine();
double getSoundDuration();
double getCurrentSoundDuration();
void setVolume(float newVolume);
void setSoundDuration(float newSoundDuration);

#endif


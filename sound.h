#ifndef SOUND_H
#define SOUND_H

#include <string>

bool playMusic();
void pauseMusic();
void resumeMusic();
double getSoundDuration();
double getCurrentSoundDuration();
void setVolume(float newVolume);
void setSoundDuration(float newSoundDuration);
void playNextSong();
void playPreviousSong();
void songEndCallback();

#endif


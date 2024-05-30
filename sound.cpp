#include "sound.h"
#include "globals.h"
#include <iostream>

using namespace irrklang;

ISoundEngine* engine = nullptr;
ISound* sound = nullptr;

float previousVolumeSound = 1.0f;

bool initSoundEngine() 
{
  engine = createIrrKlangDevice();
  return engine != nullptr;
}

bool playMusic() 
{
  if (engine) 
  {
    if (sound) 
    {
      previousVolumeSound = sound->getVolume();
    }

    cleanupSoundEngine();
    initSoundEngine();
    sound = engine->play2D(filePaths[songIndex].c_str(), false, true, true);
    sound->setIsPaused(false);
    sound->setVolume(previousVolumeSound);

    return true;
  }
  return false;
}

void pauseMusic() 
{
  if (sound)
    sound->setIsPaused(true);
}

void resumeMusic() 
{
  if (sound)
    sound->setIsPaused(false);
}

void cleanupSoundEngine() 
{
  if (sound)
    sound->drop();
  if (engine)
    engine->drop();
}

double getSoundDuration()
{
  if (sound)
    return sound->getPlayLength() / 1000;
  return -1;
}

double getCurrentSoundDuration()
{
  if (sound)
    return sound->getPlayPosition() / 1000;
  return -1;
}

void setVolume(float newVolume)
{
  sound->setVolume(newVolume);
}

void setSoundDuration(float newSoundDuration)
{
  sound->setPlayPosition(newSoundDuration * 1000);
}

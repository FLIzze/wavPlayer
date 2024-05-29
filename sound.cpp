#include "sound.h"
#include "globals.h"
#include <iostream>

using namespace irrklang;

ISoundEngine* engine = nullptr;
ISound* sound = nullptr;

bool initSoundEngine() 
{
  engine = createIrrKlangDevice();
  return engine != nullptr;
}

bool playMusic() 
{
  if (engine) 
  {
    cleanupSoundEngine();
    initSoundEngine();
    sound = engine->play2D(filePaths[songIndex].c_str(), false, true, true);
    sound->setIsPaused(false);
    return sound != nullptr;
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

int getSoundDuration()
{
  if (sound)
    return sound->getPlayLength() / 1000;
  return -1;
}

int getCurrentSoundDuration()
{
  if (sound)
    return sound->getPlayPosition() / 1000;
  return -1;
}

void setVolume(int newVolume)
{
  std::cout << "Setting volume." << std::endl;
  sound->setVolume(newVolume / 100);
}

#include "sound.h"
#include "globals.h"
#include <iostream>
#include "gui.h"
#include <SFML/Audio.hpp>

float previousVolumeSound = 1.0f;
sf::Music sound;

void playNextSong()
{
  if (songIndex + 1 < filePaths.size()) 
  {
    songIndex += 1;
    playMusic();
    updateBox();
  } else 
  {
    std::cerr << "There is no next song end of index." << std::endl;
  }
}

void playPreviousSong()
{
  if (songIndex > 0)
  {
    songIndex -= 1;
    playMusic();
    updateBox();
  } else 
  {
    std::cerr << "There is no previous song, start of index." << std::endl;
  }
}

bool playMusic() 
{
  if (sound.openFromFile(filePaths[songIndex]))
  {
    sound.play();
    return true;
  }
  return false;
}

void pauseMusic() 
{
  sound.pause();
}

void resumeMusic() 
{
  sound.play();
}

double getSoundDuration()
{
  sf::Time duration = sound.getDuration();
  return duration.asSeconds();
}

double getCurrentSoundDuration()
{
  sf::Time offset = sound.getPlayingOffset();
  return offset.asSeconds();
}

void setVolume(float newVolume)
{
  sound.setVolume(newVolume);
}

void setSoundDuration(float newSoundDuration)
{
  sf::Time timeOffset = sf::seconds(newSoundDuration);
  sound.setPlayingOffset(timeOffset);
}

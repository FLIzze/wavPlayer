#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Timer.H> 

#include "globals.h"
#include "sound.h"

Fl_Window* window;
Fl_Button* playButton;
Fl_Button* pauseButton;
Fl_Button* nextButton;
Fl_Button* previousButton;
Fl_Box* box;
Fl_Box* imageBox;
Fl_Output* duration;

const char* returnFormattedSongName()
{
  const char* songName = filePaths[songIndex].c_str();
  return songName;
}

void updateBox()
{
  box->label(returnFormattedSongName());
  window->redraw();
}

void pauseCallback(Fl_Widget*, void*) 
{
  pauseMusic();
}

void resumeCallback(Fl_Widget*, void*) 
{
  resumeMusic();
}

void nextSongCallback(Fl_Widget*, void*)
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

void previousSongCallback(Fl_Widget*, void*)
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

void updateDuration(Fl_Widget* widget) 
{
  duration->value((std::to_string(getCurrentSoundDuration()) + " / " + std::to_string(getSoundDuration())).c_str());
  window->redraw();
}

void timerCallback(void* data) 
{
  Fl_Widget* widget = static_cast<Fl_Widget*>(data);
  updateDuration(widget);
  Fl::repeat_timeout(1.0, timerCallback, data); 
}

void displayWindow() 
{
  window = new Fl_Window(700, 500, "Music Player");
  window->begin();

  playButton = new Fl_Button(0, 0, 120, 30, "Play");
  playButton->callback(resumeCallback);

  pauseButton = new Fl_Button(130, 0, 120, 30, "Pause");
  pauseButton->callback(pauseCallback);

  previousButton = new Fl_Button(0, 120, 100, 30, "Previous");
  previousButton->callback(previousSongCallback);

  nextButton = new Fl_Button(130, 120, 100, 30, "Next");
  nextButton->callback(nextSongCallback);

  box = new Fl_Box(0, 40, 500, 30, returnFormattedSongName());

  duration = new Fl_Output(0, 80, 250, 30, "Sound Duration:");
  duration->value((std::to_string(getCurrentSoundDuration()) + " / " + std::to_string(getSoundDuration())).c_str());

  Fl::add_timeout(1.0, timerCallback, duration); 

  window->end();
  window->show();
}

void runFLTK() {
  Fl::run();
}


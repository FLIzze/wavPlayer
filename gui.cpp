#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Timer.H> 
#include <FL/Fl_Value_Slider.H>

#include "globals.h"
#include "sound.h"

Fl_Window* window;
Fl_Button* playButton;
Fl_Button* pauseButton;
Fl_Button* nextButton;
Fl_Button* previousButton;
Fl_Box* box;
Fl_Box* imageBox;
Fl_Value_Slider* duration;
Fl_Value_Slider* volumeSlider;

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

void songsInputCallback(Fl_Widget* widget, void*)
{
  for (int i = 0; i < filePaths.size(); i ++)
  {
    if (widget->label() == filePaths[i])
    {
      songIndex = i;
      updateBox();
    }
  }
  playMusic();
}

void pauseCallback(Fl_Widget*, void*) 
{
  pauseMusic();
}

void resumeCallback(Fl_Widget*, void*) 
{
  resumeMusic();
}

void volumeSliderCallback(Fl_Widget* widget, void*)
{
  Fl_Value_Slider* slider = (Fl_Value_Slider*)widget;
  slider->value(slider->value());
  setVolume(slider->value() / 100); 
}

void durationSliderCallback(Fl_Widget* widget)
{
  Fl_Value_Slider* duration = (Fl_Value_Slider*)widget;
  duration->value(getCurrentSoundDuration());
  window->redraw();
}

void setDurationCallback(Fl_Widget* widget)
{
  Fl_Value_Slider* duration = (Fl_Value_Slider*)widget;
  duration->range(0, getSoundDuration());
 setSoundDuration(duration->value());
 window->redraw();
}

void timerCallback(void* data) 
{
  Fl_Widget* widget = static_cast<Fl_Widget*>(data);
  durationSliderCallback(widget);
  Fl::repeat_timeout(1.0, timerCallback, data); 
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

void displayWindow() 
{
  window = new Fl_Window(700, 500, "Music Player");
  window->begin();

  playButton = new Fl_Button(0, 0, 120, 30, "Play");
  playButton->callback(resumeCallback);

  pauseButton = new Fl_Button(130, 0, 120, 30, "Pause");
  pauseButton->callback(pauseCallback);

  previousButton = new Fl_Button(0, 150, 100, 30, "Previous");
  previousButton->callback(previousSongCallback);

  nextButton = new Fl_Button(130, 150, 100, 30, "Next");
  nextButton->callback(nextSongCallback);

  box = new Fl_Box(0, 40, 500, 30, returnFormattedSongName());

  for (int i = 0; i < filePaths.size(); i++)
  {
    Fl_Button* song = new Fl_Button(0, (200+(i+30)*i), 700, 30, filePaths[i].c_str());
    song->callback(songsInputCallback);
  }

  duration = new Fl_Value_Slider(0, 80, 400, 30, "Sound Duration:");
  duration->type(FL_HOR_SLIDER);
  duration->step(1);
  duration->value(0);
  duration->range(0, getSoundDuration());
  duration->callback(setDurationCallback);

  volumeSlider = new Fl_Value_Slider(300, 150, 400, 30, "Volume");
  volumeSlider->type(FL_HOR_SLIDER);
  volumeSlider->range(0, 100);
  volumeSlider->value(100);
  volumeSlider->step(1);
  volumeSlider->callback(volumeSliderCallback);

  Fl::add_timeout(1.0, timerCallback, duration); 

  window->end();
  window->show();
}

void runFLTK() {
  Fl::run();
}

#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Timer.H> 
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_PNG_Image.H>

#include "globals.h"
#include "sound.h"
#include "metadata.h"

#include <map>
#include <string>

using namespace std;

Fl_Window* window;
Fl_Button* playButton;
Fl_Button* pauseButton;
Fl_Button* nextButton;
Fl_Button* previousButton;
Fl_Box* artistBox;
Fl_Box* titleBox;
Fl_Box* imageBox;
Fl_Value_Slider* duration;
Fl_Value_Slider* volumeSlider;
Fl_PNG_Image pause("./assets/pause.png");
Fl_PNG_Image resume("./assets/play.png");

string title;
string artist;

void updateBox()
{
  if (!metaDatas.empty() && songIndex >= 0 && songIndex < metaDatas.size())
  {
    title = metaDatas[songIndex]["title"];
    artist = metaDatas[songIndex]["artist"];

    artistBox->label(artist.c_str());
    titleBox->label(title.c_str());

    window->redraw();
  }
  else
  {
    std::cerr << "Error: metaDatas is empty or songIndex is out of bounds" << std::endl;
  }
}

void songsInputCallback(Fl_Widget* widget, void*)
{
  for (int i = 0; i < filePaths.size(); i ++)
  {
    if (widget->label() == metaDatas[i]["title"])
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
  setVolume(slider->value()); 
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
  if (getCurrentSoundDuration() >= getSoundDuration())
  {
    playNextSong();
  }
  Fl::repeat_timeout(1.0, timerCallback, data); 
}


void nextSongCallback(Fl_Widget*, void*)
{
  playNextSong();
}


void previousSongCallback(Fl_Widget*, void*)
{
  playPreviousSong();
}


void displayWindow() 
{
  int windowHeight = 500;
  int windowWidth = 700;

  window = new Fl_Window(windowWidth, windowHeight, "Music Player");
  window->color(FL_WHITE);
  window->begin();

  playButton = new Fl_Button(0, 450, 30, 30);
  playButton->image(pause);
  playButton->callback(resumeCallback);

  pauseButton = new Fl_Button(130, 450, 30, 30);
  pauseButton->image(resume);
  pauseButton->callback(pauseCallback);

  previousButton = new Fl_Button(0, 150, 100, 30, "Previous");
  previousButton->callback(previousSongCallback);

  nextButton = new Fl_Button(130, 150, 100, 30, "Next");
  nextButton->callback(nextSongCallback);

  for (int i = 0; i < filePaths.size(); i++)
  {
    Fl_Button* song = new Fl_Button(0, (200+i*30), 700, 30, metaDatas[i]["title"].c_str());
    song->callback(songsInputCallback);
  }

  titleBox = new Fl_Box(0, 20, windowWidth, 30, "title");
  titleBox->labelsize(21);

  artistBox = new Fl_Box(0, 40, windowWidth, 30, "artist");
  Fl::add_timeout(0.02, updateArtistPosition, NULL);

  updateBox();

  duration = new Fl_Value_Slider(0, 90, 400, 30);
  duration->type(FL_HOR_SLIDER);
  duration->step(1);
  duration->value(0);
  duration->range(0, getSoundDuration());
  duration->callback(setDurationCallback);

  volumeSlider = new Fl_Value_Slider(300, 150, 400, 30);
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

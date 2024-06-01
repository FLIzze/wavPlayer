#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Timer.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Scroll.H>
#include <FL/fl_draw.H>

#include <algorithm>
#include <map>
#include <string>

#include "globals.h"
#include "sound.h"
#include "metadata.h"

using namespace std;

Fl_Window* window;
Fl_Button* playButton;
Fl_Button* pauseButton;
Fl_Button* nextButton;
Fl_Button* previousButton;
Fl_Box* artistBox;
Fl_Box* titleBox;
Fl_Box* lyricsBox;
Fl_Box* coverImage;
Fl_Box* currentDurationBox;
Fl_Slider* duration;
Fl_Slider* volumeSlider;
Fl_Button* muteButton;
Fl_Button* songButton;
Fl_PNG_Image pauseIcon("./assets/pause.png");
Fl_PNG_Image resumeIcon("./assets/play.png");
Fl_PNG_Image nextIcon("./assets/next.png");
Fl_PNG_Image previousIcon("./assets/previous.png");
Fl_PNG_Image coverImageIcon("./assets/covers/hazbin.png");
Fl_Scroll* lyricsScroll;
Fl_Scroll* songListScroll;

string title;
string artist;
string lyrics;
string currentDurationString;

void updateBox() {
    if (!metaDatas.empty() && songIndex >= 0 && songIndex < metaDatas.size()) {
        title = metaDatas[songIndex]["title"];
        artist = metaDatas[songIndex]["artist"];
        lyrics = metaDatas[songIndex]["lyrics"];

        int lyricsWidth = lyricsScroll->w() - 20;
        int lines = count(lyrics.begin(), lyrics.end(), '\n') + 1;
        int lineHeight = fl_height();
        int lyricsHeight = lineHeight * lines;
        lyricsBox->size(lyricsWidth, lyricsHeight);

        artistBox->label(artist.c_str());
        titleBox->label(title.c_str());
        lyricsBox->label(lyrics.c_str());
    } else {
        std::cerr << "Error: metaDatas is empty or songIndex is out of bounds" << std::endl;
    }

    duration->value(getCurrentSoundDuration());
    window->redraw();

    int currentDuration = getCurrentSoundDuration();
    int totalDuration = getSoundDuration();

    currentDurationString = to_string(currentDuration) + " / " + to_string(totalDuration);

    currentDurationBox->label(currentDurationString.c_str());

    if (currentDuration + 1 >= totalDuration) {
        playNextSong();
    }
}

void playSongCallback(Fl_Widget* widget, void*)
{
  for (int i = 0; i < filePaths.size(); i++)
  {
    if (metaDatas[i]["title"] == widget->label())
    {
      songIndex = i;
      break;
    }
  }
  playMusic();
  updateBox();
}

void muteButtonCallback(Fl_Widget*, void*) {
    mute();
}

void songsInputCallback(Fl_Widget* widget, void*) {
    for (int i = 0; i < filePaths.size(); i++) {
        if (widget->label() == metaDatas[i]["title"]) {
            songIndex = i;
            updateBox();
        }
    }
    playMusic();
}

void resumeCallback(Fl_Widget* widget, void*) {
    if (isPause) {
        widget->label("pause");
        resumeMusic();
    } else {
        widget->label("play");
        pauseMusic();
    }
    std::cout << "resumeCallback" << endl;
}

void volumeSliderCallback(Fl_Widget* widget, void*) {
    Fl_Value_Slider* slider = (Fl_Value_Slider*)widget;
    slider->value(slider->value());
    setVolume(slider->value());
}

void durationSliderCallback(Fl_Widget* widget) {
    Fl_Value_Slider* duration = (Fl_Value_Slider*)widget;
    duration->value(getCurrentSoundDuration());
    window->redraw();
}

void setDurationCallback(Fl_Widget* widget) {
    Fl_Value_Slider* duration = (Fl_Value_Slider*)widget;
    duration->range(0, getSoundDuration());
    setSoundDuration(duration->value());
    window->redraw();
}

void timerCallback(void* data) {
    std::cout << "timerCallback call" << endl;

    Fl_Widget* widget = static_cast<Fl_Widget*>(data);

    updateBox();

    Fl::repeat_timeout(1.0, timerCallback, data);
}

void nextSongCallback(Fl_Widget*, void*) {
  playNextSong();
}

void previousSongCallback(Fl_Widget*, void*) {
  playPreviousSong();
}

void displayWindow() {
    int windowWidth = 1920;
    int windowHeight = 1080;

    window = new Fl_Window(windowWidth, windowHeight, "Music Player");
    window->color(FL_WHITE);
    window->begin();

    playButton = new Fl_Button(50, 150, windowWidth, 50, "play");
    playButton->callback(resumeCallback);
    playButton->labelsize(30);
    playButton->box(FL_NO_BOX);

    previousButton = new Fl_Button(windowWidth / 2 -200, 150, 100, 50, "previous");
    previousButton->labelsize(30);
    //previousButton->image(previousIcon);
    previousButton->callback(previousSongCallback);
    previousButton->box(FL_NO_BOX);

    nextButton = new Fl_Button(windowWidth / 2 + 200, 150, 100, 50, "next");
    nextButton->labelsize(30);
    //nextButton->image(nextIcon);
    nextButton->callback(nextSongCallback);
    nextButton->box(FL_NO_BOX);

    titleBox = new Fl_Box(0, 20, windowWidth, 30, "title");
    titleBox->labelsize(21);

    artistBox = new Fl_Box(0, 40, windowWidth, 30, "artist");

    duration = new Fl_Slider(0, 90, windowWidth, 30);
    duration->type(FL_HOR_SLIDER);
    duration->step(1);
    duration->value(0);
    duration->range(0, getSoundDuration());
    duration->callback(setDurationCallback);

    currentDurationBox = new Fl_Box(30, 130, 30, 30, "duration");

    volumeSlider = new Fl_Slider(0, windowHeight-30, windowWidth, 30);
    volumeSlider->type(FL_HOR_SLIDER);
    volumeSlider->range(0, 100);
    volumeSlider->value(100);
    volumeSlider->step(1);
    volumeSlider->callback(volumeSliderCallback);

    lyricsScroll = new Fl_Scroll(0, 210, windowWidth / 2, windowHeight-270);
    lyricsScroll->box(FL_DOWN_BOX);
    lyricsBox = new Fl_Box(0, 0, 0, 0, "");
    lyricsBox->box(FL_NO_BOX);
    lyricsScroll->end();

    songListScroll = new Fl_Scroll(windowWidth / 2, 210, windowWidth / 2, windowHeight-270);
    songListScroll->box(FL_DOWN_BOX);
    
    int buttonY = 0;
    for (int i = 0; i < filePaths.size(); i++)
    {
      songButton = new Fl_Button(0, buttonY, windowWidth / 2, 20, metaDatas[i]["title"].c_str());
      songButton->callback(playSongCallback);
      songListScroll->add(songButton);
      buttonY += 20; 
    }

    songListScroll->scroll_to(0, 0);
    songListScroll->end();

    updateBox();

    Fl::add_timeout(1.0, timerCallback, duration);

    window->end();
    window->show();
}

void runFLTK() {
    Fl::run();
}

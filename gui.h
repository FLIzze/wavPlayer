#ifndef GUI_H
#define GUI_H

void displayWindow();
void runFLTK();
void previousSong();
void nextSong();
void updateBox();
void updateDuration();
char* returnFormattedSongName();
void nextSongCallback();
void previousSongCallback();
void timerCallback();
void volumeSliderCallback(); 

#endif // GUI_r


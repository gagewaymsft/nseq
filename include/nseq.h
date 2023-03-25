#include <ncurses.h>
#include "clip.h"
#include "track.h"
#include "audioEngine.h"
#include "gui.h"
#include "nsequeue.h"
#include <vector>
#include <string>

bool newFile();
bool addCue();
bool playCue();
bool stopPlaying();
bool doAllEvents();
bool addTrack();
bool deleteTrack();
bool deleteClip();
bool deleteCue();
int addClip(int type);
std::vector<std::string> getTrackNames();
bool selectClip(const int t, const int c);
void updateArrangerFrame(bool active);
void updateDetailFrame(bool active);
void updateMaxClips();
bool updateCues();
void toggleActiveFrame();
void menuLeft();
void menuRight();
void menuUp();
void menuDown();
bool nextClipType();
void redrawScreen();
bool editClipName();
bool editTrackName();

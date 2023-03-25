#include <unistd.h>
#include <ncurses.h>
#include <algorithm>
#include <vector>
#include <string>
#include "track.h"

//int screenWidth = 0;
//int screenHeight = 0;
//WINDOW * mainwin, * arrangerwin, * detailwin;

int startcurses();
int initMainWindow();
int initArrangerFrame();
int initDetailFrame();
void drawArrangerGrid(const int s, const std::vector<std::string> trackNames, const std::vector<std::vector<std::string>> clipNames, const int ht, const int hc);
int drawArrangerFrame(bool active, const int s, const std::vector<std::string> trackNames, const std::vector<std::vector<std::string>> clipNames, const int ht, const int hc);
int drawMainWindowHeader(const char * header);
int drawTempoHeader(int tempo);
int drawBeatHeader(int beat);
void drawOctaveNavBar(int startOctave);
void drawDetailGridNavBar(double p);
void drawDetailGrid(bool notes);
int drawDetailFrame(bool active, int type, std::string typeName, std::string trackName, std::string clipName, int menuX, int menuY);
int handleKeys();
std::string getStringInput();
int endcurses();
void printDebug(std::string str);
void drawSampler(int h);
void drawSequencer(int h);
void drawFade();
void drawConfig();
std::string showTextPopup(const std::string header, const std::string content);
void closeTextPopup();
void drawPopupHeader(int winWidth, const std::string header);
void drawTextPopupOptions(int h, int w, const std::string optionA, const std::string optionB, bool highlightA, bool highlightB);
void drawStringBuffer(int h, int w, const char* str);
void updateStringBuffer(int h, int w, char s);

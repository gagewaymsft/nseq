#include <gui.h>
#include <cstring>
#include <algorithm>

int screenWidth = 0;
int screenHeight = 0;
WINDOW * mainwin, * arrangerwin, * detailwin, *textpopup, *textpopupstring;
char strBuf[100];

int startcurses()
{
  initscr();
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  init_pair(2, COLOR_RED, COLOR_BLACK);
  init_pair(3, COLOR_GREEN, COLOR_BLACK);
  init_pair(4, COLOR_YELLOW, COLOR_BLACK);
  init_pair(5, COLOR_BLUE, COLOR_BLACK);
  init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(7, COLOR_CYAN, COLOR_BLACK);
  nodelay(stdscr, TRUE);
  noecho();
  curs_set(FALSE);
  getmaxyx(stdscr, screenHeight, screenWidth);
  return 0;
}


int initMainWindow()
{
  mainwin = newwin(screenHeight, screenWidth, 0, 0);
  box(mainwin, 0, 0);
  wrefresh(mainwin);
  return 0;
}

int initArrangerFrame()
{
  arrangerwin = subwin(mainwin, screenHeight-22, screenWidth-2,1,1);
  wrefresh(arrangerwin);
  return 0;
}

int initDetailFrame()
{
  detailwin = subwin(mainwin, 20,screenWidth,screenHeight-20,0);
  box(detailwin, 0, 0);
  wrefresh(detailwin);
  return 0;
}

void drawStringBuffer(int h, int w, const char* str)
{
  curs_set(TRUE);
  wclear(textpopupstring);
  mvwprintw(textpopupstring, h, w, str);
  move(h, std::strlen(str) + 1);
  wrefresh(textpopupstring);
}

void updateStringBuffer(int h, int w, char s)
{
  std::string str(strBuf);
  str += s;
  sprintf(strBuf, "%s", str.c_str());
//  strBuf[std::strlen(strBuf) + 1] = s;
  drawStringBuffer(h, w, strBuf);
}

void drawTextPopupOptions(int h, int w, const std::string optionA, const std::string optionB, bool highlightA, bool highlightB)
{
  if(highlightA)
    wattron(textpopup, A_REVERSE);
  wattron(textpopup, COLOR_PAIR(5));
  mvwprintw(textpopup, h, w - ((int) std::strlen(optionA.c_str()) + (int) std::strlen(optionB.c_str()) + 8), optionA.c_str());
  if(highlightA)
    wattroff(textpopup, A_REVERSE);
  wattroff(textpopup, COLOR_PAIR(5));

  if(highlightB)
    wattron(textpopup, A_REVERSE);
  wattron(textpopup, COLOR_PAIR(2));
  mvwprintw(textpopup, h, w - ((int) std::strlen(optionB.c_str()) + 4), optionB.c_str());
  if(highlightB)
    wattroff(textpopup, A_REVERSE);
  wattroff(textpopup, COLOR_PAIR(2));
}

void drawPopupHeader(int winWidth, const std::string header)
{
  box(textpopup, 0, 0);
  mvwprintw(textpopup, 0, (winWidth - (int) std::strlen(header.c_str())) / 2, header.c_str());
}

std::string showTextPopup(const std::string header, const std::string content)
{
  int winWidth = std::max(screenWidth / 4, (int)std::strlen(content.c_str()) + 8);
  int winHeight = 6;
  int winYPos = (screenHeight - winHeight) / 2;
  int winXPos = (screenWidth - winWidth) / 2;
  int c;
  int highlight = 0;
  int cursPos = (int) std::strlen(content.c_str()) + 1;
  bool cancel = false;
  std::string output = "";
  sprintf(strBuf, "%s", content.c_str());
  textpopup = newwin(winHeight, winWidth, winYPos, winXPos);
  textpopupstring = newwin(3, winWidth - 2, winYPos + 1, winXPos + 1);
  drawPopupHeader(winWidth, header);
//  touchwin(textpopup);
//  mvwprintw(textpopup, 2, 2, strBuf);
  drawTextPopupOptions(4, winWidth, "OK", "CANCEL", false, false);
  wrefresh(textpopup);
  drawStringBuffer(1,1, strBuf);

  bool b = true;
  while(b)
  {
    c = wgetch(textpopup);
    switch(c)
    {
      case 10:
        b = false;
        break;
//      case 27:
//        b = true;
//        cancel = true;
//        break;
      case KEY_RIGHT: case KEY_DOWN:
        highlight ++;
        if(highlight >= 3)
          highlight = 0;
        if(highlight == 1)
        {
          drawTextPopupOptions(4, winWidth, "OK", "CANCEL", true, false);
        }
        else if(highlight == 2)
        {
          drawTextPopupOptions(4, winWidth, "OK", "CANCEL", false, true);
        }
        else
        {
//          drawPopupHeader(winWidth, header);
          drawTextPopupOptions(4, winWidth, "OK", "CANCEL", false, false);
//          drawStringBuffer(2,2, strBuf);
        }
        break;
      case KEY_LEFT: case KEY_UP:
        highlight --;
        if(highlight < 0)
          highlight = 2;
        if(highlight == 1)
        {
          drawTextPopupOptions(4, winWidth, "OK", "CANCEL", true, false);
        }
        else if(highlight == 2)
        {
          drawTextPopupOptions(4, winWidth, "OK", "CANCEL", false, true);
        }
        else
        {
//          drawPopupHeader(winWidth, header);
          drawTextPopupOptions(4, winWidth, "OK", "CANCEL", false, false);
//          drawStringBuffer(2,2, strBuf);
        }
        break;
      case 127: //backspace
//        drawPopupHeader(winWidth, header);
//        drawTextPopupOptions(4, winWidth, "OK", "CANCEL", false, false);
        strBuf[std::strlen(strBuf) - 1] = '\0';
        drawStringBuffer(1,1, strBuf);
        break;
      default:
        updateStringBuffer(1,1,c);
        break;
    }
//    if (b)
//      break;
  }
  closeTextPopup();
  if(cancel)
    return "";

  output = strBuf;
  return output;
}

void closeTextPopup()
{
  delwin(textpopup);
  delwin(textpopupstring);
  clear();
  refresh();
  curs_set(FALSE);
}

void drawArrangerGrid(const int s, const std::vector<std::string> trackNames, const std::vector<std::vector<std::string>> clipNames, const int ht, const int hc)
{
  int maxTrax = std::max(std::min(s, screenWidth / 9), 0);
//  int track = ht+1;
//  int clip = hc+1;
//  int highlightTrack = ht;
//  int highlightClip = hc;
  for(int i = 0; i < screenWidth -2; i++)
  {
    for(int j = 0; j < screenHeight-22; j++)
    {
      if(i == 0)
      {
        if(j > 0)
        {
          if(j == hc)
            wattron(arrangerwin, A_REVERSE);
          mvwprintw(arrangerwin,j,i,"%X",j-1);
          if(j == hc)
            wattroff(arrangerwin, A_REVERSE);
        }
      }
      else if((i-2) % 9 == 0)
      {
        mvwaddch(arrangerwin,j,i,ACS_VLINE);
      }
    }
  }
  for (int i = 0; i < trackNames.size(); i++)
  {
    char tempName[] = "        ";
    sprintf(tempName, "%-8.8s", trackNames.at(i).c_str());
    if(i == ht && hc == 0)
      wattron(arrangerwin, A_REVERSE);
    wattron(arrangerwin, COLOR_PAIR(i+2));
    mvwprintw(arrangerwin,0,i+3+(i*8),tempName);
    if(i == ht && hc == 0)
      wattroff(arrangerwin, A_REVERSE);
    for(int j = 1; j < clipNames.at(i).size(); j++)
    {
      if(clipNames.at(i).at(j) == "")
      {
        mvwprintw(arrangerwin,j+1,i+3+(i*8),"<blank>");
      }
      else
      {
        if(i == ht && j == hc)
        {
          wattron(arrangerwin, A_REVERSE);
        }
        sprintf(tempName, "%-8.8s", clipNames.at(i).at(j).c_str());
        mvwprintw(arrangerwin,j,i+3+(i*8),tempName);
        if(i == ht && j == hc)
        {
          wattroff(arrangerwin, A_REVERSE);
        }
      }
    }
    wattroff(arrangerwin, COLOR_PAIR(i+2));
  }
}

int drawArrangerFrame(bool active, const int s, const std::vector<std::string> trackNames, const std::vector<std::vector<std::string>> clipNames, const int ht, const int hc)
{
  drawArrangerGrid(s,trackNames,clipNames,ht,hc);
  wrefresh(arrangerwin);
  return 0;
}

int drawMainWindowHeader(const char * header)
{
  box(mainwin, 0, 0);
  int startPos = (screenWidth / 2) - (sizeof(header) / 2) - 1;
  int endPos = startPos + sizeof(header) + 1;
//  wattron(mainwin, A_REVERSE);
  mvwprintw(mainwin, 0, startPos, "[%s]", header);
//  wattroff(mainwin, A_REVERSE);
  wrefresh(mainwin);
  return 0;
}

int drawTempoHeader(int tempo)
{
//  int tempo = 105;
//  wattron(mainwin, A_REVERSE);
  mvwprintw(mainwin, 0, screenWidth - 20, "[tempo: %03d bpm]", tempo);
//  wattroff(mainwin, A_REVERSE);
  wrefresh(mainwin);
  return 0;
}

int drawBeatHeader(int beat)
{
  mvwprintw(mainwin, 0, screenWidth - 40, "[|---|---|---|---]");
  mvwaddch(mainwin, 0, beat + screenWidth - 39, '*');
//  wattroff(mainwin, A_REVERSE);
  wrefresh(mainwin);
  return 0;
}

void drawOctaveNavBar(int startOctave)
{
  wattron(detailwin, A_REVERSE);
  for (int i = 2; i < 16; i++)
  {
    if (i == 2)
    {
      mvwaddch(detailwin,i, screenWidth - 19,ACS_UARROW);
    }
    else if (i == 15)
    {
      mvwaddch(detailwin,i, screenWidth - 19,ACS_DARROW);
    }
    else
    {
      mvwaddch(detailwin,i, screenWidth - 19,ACS_CKBOARD);
    }
  }
  wattroff(detailwin, A_REVERSE);
}

void drawDetailGridNavBar(double p)
{
//  int currentPos = ((double)screenWidth - 20) * p;
  wattron(detailwin, A_REVERSE);
  for(int i = 2; i < screenWidth - 20; i++)
  {
    if(i == 2)
    {
      mvwaddch(detailwin,16,i,ACS_LARROW);
    }
    else if (i == screenWidth - 21)
    {
      mvwaddch(detailwin,16,i,ACS_RARROW);
    }
    else
    {
      mvwaddch(detailwin,16,i,ACS_CKBOARD);
    }
  }
  wattroff(detailwin, A_REVERSE);

}

void drawDetailGrid(bool notes)
{
  wattron(detailwin, COLOR_PAIR(7));
  for(int i = 2; i < screenWidth - 20; i++)
  {

    for(int j = 2; j < 15; j++)
    {
      if (i == 2)
      {
        if(notes)
        {
          if(j == 2)
          {
            mvwaddch(detailwin,j,i,'c');
            mvwaddch(detailwin,j,i+1,'4');
          }
          else if(j == 3)
          {
            mvwaddch(detailwin,j,i,'b');
          }
          else if(j == 4)
          {
            mvwaddch(detailwin,j,i,'a');
            mvwaddch(detailwin,j,i+1,'#');
          }
          else if(j == 5)
          {
            mvwaddch(detailwin,j,i,'a');
          }
          else if(j == 6)
          {
            mvwaddch(detailwin,j,i,'g');
            mvwaddch(detailwin,j,i+1,'#');
          }
          else if(j == 7)
          {
            mvwaddch(detailwin,j,i,'g');
          }
          else if(j == 8)
          {
            mvwaddch(detailwin,j,i,'f');
            mvwaddch(detailwin,j,i+1,'#');
          }
          else if(j == 9)
          {
            mvwaddch(detailwin,j,i,'f');
          }
          else if(j == 10)
          {
            mvwaddch(detailwin,j,i,'e');
          }
          else if(j == 11)
          {
            mvwaddch(detailwin,j,i,'d');
            mvwaddch(detailwin,j,i+1,'#');
          }
          else if(j == 12)
          {
            mvwaddch(detailwin,j,i,'d');
          }
          else if(j == 13)
          {
            mvwaddch(detailwin,j,i,'c');
            mvwaddch(detailwin,j,i+1,'#');
          }
          else if(j == 14)
          {
            mvwaddch(detailwin,j,i,'c');
            mvwaddch(detailwin,j,i+1,'3');
          }
        }
      }
      else if (i == 3)
      {
      }
      else if ((i - 4) % 16 == 0)
      {
        mvwaddch(detailwin,j,i,'|');
      }
      else
      {
        mvwaddch(detailwin,j,i,'-');
      }
    }
  }
  wattroff(detailwin, COLOR_PAIR(7));
}

void drawConfig()
{
}

void drawSequencer(int h)
{
  drawDetailGrid(true);
  drawDetailGridNavBar(0);
  drawOctaveNavBar(3);
  wattron(detailwin, A_REVERSE);
  mvwprintw(detailwin, 2, screenWidth - 15, " INSTRUMENT ");
  mvwprintw(detailwin, 4, screenWidth - 15, "   VOLUME   ");
  mvwprintw(detailwin, 6, screenWidth - 15, "   PANNING  ");
  mvwprintw(detailwin, 8, screenWidth - 15, "   EFFECTS  ");
  wattroff(detailwin, A_REVERSE);
  mvwprintw(detailwin, 3, screenWidth - 15, "sampler");
  mvwprintw(detailwin, 5, screenWidth - 15, "0");
  mvwprintw(detailwin, 7, screenWidth - 15, "C");
  mvwprintw(detailwin, 9, screenWidth - 15, "none");
}

void drawSampler(int h)
{
  drawDetailGrid(false);
  drawDetailGridNavBar(0);
  wattron(detailwin, A_REVERSE);
  mvwprintw(detailwin, 2, screenWidth - 15, " INSTRUMENT ");
  mvwprintw(detailwin, 4, screenWidth - 15, "   VOLUME   ");
  mvwprintw(detailwin, 6, screenWidth - 15, "   PANNING  ");
  mvwprintw(detailwin, 8, screenWidth - 15, "   EFFECTS  ");
  wattroff(detailwin, A_REVERSE);
  if(h == 3)
    wattron(detailwin, A_REVERSE);
  mvwprintw(detailwin, 3, screenWidth - 15, "sample.wav");
  if(h == 3)
    wattroff(detailwin, A_REVERSE);
  mvwprintw(detailwin, 5, screenWidth - 15, "0");
  mvwprintw(detailwin, 7, screenWidth - 15, "C");
  mvwprintw(detailwin, 9, screenWidth - 15, "none");
}

void drawFade()
{
}

int drawDetailFrame(bool active, int type, std::string typeName, std::string trackName, std::string clipName, int menuX, int menuY)
{
//  box(detailwin,0,0);
//  int headerBar = std::strlen(trackName) + std::strlen(clipName) + 10;
  wclear(detailwin);
  box(detailwin, 0, 0);
  int headerBar = screenWidth - 1;
  if(active)
  {
    wattron(detailwin, A_REVERSE);
//    wattron(detailwin, COLOR_PAIR(5));
  }
  for(int i = 1; i < headerBar; i++)
  {
    mvwaddch(detailwin,0,i,ACS_HLINE);
  }
  if(active)
    wattroff(detailwin, A_REVERSE);
//  if(active)
//  {
//    wattroff(detailwin, COLOR_PAIR(5));
//  }
  wattron(detailwin, COLOR_PAIR(4));
  if(menuX == 0 && menuY == 0 && active)
    wattron(detailwin, A_REVERSE);
  mvwprintw(detailwin, 0, 2, "[%s]", typeName.c_str());
  if(menuX == 0 && menuY == 0 && active)
    wattroff(detailwin, A_REVERSE);

  if(menuX == 1 && menuY == 0 && active)
    wattron(detailwin, A_REVERSE);
  mvwprintw(detailwin, 0, 16, "%s", trackName.c_str());
  if(menuX == 1 && menuY == 0 && active)
    wattroff(detailwin, A_REVERSE);

  if(type > 0)
  {
    if(menuX == 2 && menuY == 0 && active)
      wattron(detailwin, A_REVERSE);
    mvwprintw(detailwin, 0, sizeof(trackName) + 20, "%s", clipName.c_str());
    if(menuX == 2 && menuY == 0 && active)
      wattroff(detailwin, A_REVERSE);
  }
  wattroff(detailwin, COLOR_PAIR(4));
  switch(type)
  {
    case(0):
      drawConfig();
      break;
    case(1):
      drawSampler(menuX);
      break;
    case(2):
      drawFade();
      break;
    case(3):
      drawSequencer(menuX);
      break;
    default:
      break;
  }
  if(type == 1)
  {
  }
  wrefresh(detailwin);
  return 0;
}

int handleKeys()
{
  int ch;
  ch = wgetch(mainwin);
  return ch;
}

int endcurses()
{
  endwin();
  return 0;
}

void printDebug(std::string str)
{
  for(int i = 1; i < screenWidth -1; i++)
  {
    mvwaddch(detailwin,19,i,ACS_HLINE);
  }
  mvwprintw(mainwin, screenHeight-1, 1, ": ");
  mvwprintw(mainwin, screenHeight-1, 3, str.c_str());
  wrefresh(detailwin);
  wrefresh(mainwin);
}

std::string getStringInput()
{
  for(int i = 1; i < screenWidth -1; i++)
  {
    mvwaddch(detailwin,19,i,ACS_HLINE);
  }
  mvwprintw(mainwin, screenHeight-1, 1, ": ");
  wrefresh(detailwin);
  wrefresh(mainwin);
  return "";
}

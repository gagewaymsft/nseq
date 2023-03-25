#include <nseq.h>
#include <iostream>
#include <thread>
//#include <unistd.h>

int activeFrame = 0;
static int maxFrames = 1;
int currentTrack = 0;
int currentClip = 0;
int maxClips = 0;
int menuX = 0;
int menuY = 0;
int tick = 0;
float globalTempo = 120;
bool m_abort = false;
bool isPlaying = false;
std::string* highlightedString;
std::vector<track> tracks;
std::vector<std::vector<std::string>> clipNames;
audioEngine* engine;

void workerLoop()
{
//  int loop = 0;
  while(!m_abort)
  {
    doAllEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
//    loop++;
//    printDebug(std::to_string(loop));
  }
}

std::vector<std::string> getTrackNames()
{
  std::vector<std::string> names;
  for(int i = 0; i < tracks.size(); i++)
  {
    names.push_back(tracks.at(i).name());
  }
  return names;
}

void updateMaxClips()
{
  maxClips = 0;
  for(int i = 0; i < tracks.size(); i++)
  {
//    printDebug("track " + std::to_string(i) + ": " + std::to_string(tracks.at(i).clips.size()));
//    sleep(1);
    if(tracks.at(i).clips.size() > maxClips)
      maxClips = tracks.at(i).clips.size();
  }
}

void getClipNames()
{
  clipNames.clear();
  for(int i = 0; i < tracks.size(); i++)
  {
    clipNames.push_back(tracks.at(i).getStringList(maxClips));
  }
}

bool newFile()
{
  tracks.clear();
  addTrack();
  return true;
}

bool deleteCue()
{
  return true;
}

bool playCue()
{
  tick = 0;
  for(int i = 0; i < tracks.size(); i++)
  {
    std::vector<note> sequence = tracks.at(i).clips.at(currentClip).sequence();
    for(int j = 0; j < sequence.size(); j++)
    {
      enQueue(2, 0, sequence.at(j).startTime, sequence.at(j).endTime, sequence.at(j).pitch, sequence.at(j).velocity);
//      printDebug(std::to_string(q));
    }
  }
  enQueue(0, 0, 0, 0, 0, 0);
  isPlaying = true;
  return true;
}

bool stopPlaying()
{
  isPlaying = false;
  enQueue(-1, 0, 0, 0, 0, 0);
  return true;
}

bool addCue()
{
  for(int i = 0; i < tracks.size(); i++)
  {
    clip temp(0,-1,"none");
    tracks.at(i).clips.push_back(temp);
  }
//  printDebug("added cue");
  updateMaxClips();
//  updateCues();
  getClipNames();
//  printDebug("updated max clips");
  return true;
}

bool updateCues()
{
//  printDebug("max clips " + std::to_string(maxClips));
//  sleep(1);
  for(int i = 0; i < tracks.size(); i++)
  {
    int clipsToAdd = maxClips - tracks.at(i).clips.size();
//    printDebug("track " + std::to_string(i) + " clips to add " + std::to_string(clipsToAdd));
//    sleep(1);
    if(clipsToAdd > 0)
    {
      for(int j = 0; j < clipsToAdd; j++)
      {
        clip temp(0,-1,"none");
        tracks.at(i).clips.push_back(temp);
      }
    }
  }
  return true;
}

bool doAllEvents()
{
  std::vector<queueEvent> currentQueue = doQueue(globalTempo);
  for(int i = 0; i < currentQueue.size(); i++)
  {
    if(currentQueue.at(i).type == 2)
    {
      engine->playSample(currentQueue.at(i).sample, currentQueue.at(i).pitch);
    }
    if(currentQueue.at(i).type == 0 && isPlaying)
    {
      printDebug(std::to_string(tick));
      enQueue(0, 0, 1, 0, 0, 0);
      tick++;
      if(tick >= 16)
        tick = 0;
      drawBeatHeader(tick);
    }
  }
  return true;
}

bool deleteTrack()
{
  return true;
}

bool addTrack()
{
  track tempTrack(maxClips, "TRACK " + std::to_string(tracks.size()));
  tracks.push_back(tempTrack);
  updateMaxClips();
  updateCues();
  getClipNames();
  return true;
}

bool deleteClip()
{
  if(tracks.at(currentTrack).clips.at(currentClip).type() == 0)
    return false;

  clip tempClip(0,-1,"none");
  tracks.at(currentTrack).clips.at(currentClip) = tempClip;
  getClipNames();
  return true;
}

int addClip(int type)
{
  if(type == 0)
    return currentClip;
  if(tracks.at(currentTrack).clips.at(currentClip).type() == 0)
    return currentClip;

  bool newCue = false;
  if(tracks.at(currentTrack).clips.size() < 2 || (currentClip == maxClips - 1 && tracks.at(currentTrack).clips.at(currentClip).type() > 0))
  {
    newCue = true;
    std::string clipName = "CLIP " + std::to_string(maxClips - 1);
    clip tempClip(0,type,clipName);
    tracks.at(currentTrack).clips.push_back(tempClip);
    updateMaxClips();
    updateCues();
  }
  else
  {
    std::string clipName = "CLIP " + std::to_string(currentClip - 1);
    clip tempClip(0,type,clipName);
    tracks.at(currentTrack).clips.at(currentClip) = tempClip;
  }
  getClipNames();
  if(newCue)
    return maxClips - 1;
  return currentClip;
}

bool selectClip(const int t, const int c)
{
  if(t >= tracks.size() || t < 0)
    return false;
  if(maxClips <= 0 || c < 0)
    return false;

//  int lastClip = tracks.at(t).clips.size() - 1;
  currentTrack = t;
  currentClip = std::max(0, std::min(maxClips - 1, c));

  return true;
}

void menuLeft()
{
  menuX --;
  if(menuX < 0)
    menuX = 3;

//  highlightedString = &(*tracks.at(currentTrack).clips.at(currentClip).type());
}

void menuRight()
{
  menuX ++;
  if(menuX > 3)
    menuX = 0;
}

void menuUp()
{
}

void menuDown()
{
}

void redrawScreen()
{
        drawMainWindowHeader("NEW SONG");
        drawTempoHeader((int) globalTempo);
        updateArrangerFrame(true);
        updateDetailFrame(false);
}

bool editClipName()
{
  std::string newName = showTextPopup("header", tracks.at(currentTrack).clips.at(currentClip).name());
  if(newName != "")
  {
    tracks.at(currentTrack).clips.at(currentClip).setName(newName);
    getClipNames();
    return true;
  }

  return false;
}

bool editTrackName()
{
  std::string newName = showTextPopup("header", tracks.at(currentTrack).name());
  if(newName != "")
  {
    tracks.at(currentTrack).setName(newName);
    getClipNames();
    return true;
  }

  return false;
}


bool nextClipType()
{
  if(tracks.at(currentTrack).clips.at(currentClip).type() > 0)
  {
    tracks.at(currentTrack).clips.at(currentClip).nextClipType();
    return true;
  }
  return false;
}

void toggleActiveFrame()
{
  activeFrame++;
  if(activeFrame > maxFrames)
    activeFrame = 0;

  switch(activeFrame)
  {
    case 0:
      updateArrangerFrame(true);
      updateDetailFrame(false);
      break;
    case 1:
      menuX = 0;
      menuY = 0;
      updateArrangerFrame(false);
      updateDetailFrame(true);
      break;
    default:
      break;
  }
}

void updateArrangerFrame(bool active)
{
  drawArrangerFrame(active, tracks.size(), getTrackNames(), clipNames, currentTrack, currentClip);
}

void updateDetailFrame(bool active)
{
  drawDetailFrame(active, tracks.at(currentTrack).clips.at(currentClip).type(), tracks.at(currentTrack).clips.at(currentClip).typeString(), tracks.at(currentTrack).name(), clipNames.at(currentTrack).at(currentClip), menuX, menuY);
}

int parseKeys(int ch)
{
  bool quit = false;
  printDebug(std::to_string(ch));
  switch(ch) {
    case 9:
      toggleActiveFrame();
      break;
    case 10:
      if(activeFrame == 1)
      {
        if(menuX == 2)
        {
          editClipName();
          redrawScreen();
        }
        else if(menuX == 1)
        {
          editTrackName();
          redrawScreen();
        }

      }
      break;
    case 27:
      stopPlaying();
      break;
    case 32:
      if(activeFrame == 0)
      {
//        printDebug("play cue");
        playCue();
      }
      else if(activeFrame == 1)
      {
        nextClipType();
        updateDetailFrame(true);
      }
      break;
    case 58:
      getStringInput();
// this is going to be input mode, eventually
      break;
    case 65:
      if(activeFrame == 0)
      {
        if(selectClip(currentTrack, currentClip - 1))
        {
          updateArrangerFrame(true);
          updateDetailFrame(false);
        }
        else if(activeFrame == 1)
        {
          menuUp();
          updateDetailFrame(true);
        }
      }
      break;
    case 66:
      if(activeFrame == 0)
      {
        if(selectClip(currentTrack, currentClip + 1))
        {
          updateArrangerFrame(true);
          updateDetailFrame(false);
        }
      }
      else if(activeFrame == 1)
      {
        menuDown();
        updateDetailFrame(true);
      }
      break;
    case 67:
      if(activeFrame == 0)
      {
        if(selectClip(currentTrack + 1, currentClip))
        {
          updateArrangerFrame(true);
          updateDetailFrame(false);
        }
      }
      else if(activeFrame == 1)
      {
//        printDebug("right");
        menuRight();
        updateDetailFrame(true);
      }
      break;
    case 68:
      if(activeFrame == 0)
      {
        if(selectClip(currentTrack - 1, currentClip))
        {
          updateArrangerFrame(true);
          updateDetailFrame(false);
        }
      }
      else if(activeFrame == 1)
      {
        printDebug("left");
        menuLeft();
        updateDetailFrame(true);
      }
      break;
    case 'a':
      if(activeFrame == 0)
      {
        if(selectClip(currentTrack, addClip(1)))
        {
          updateDetailFrame(false);
        }
        updateArrangerFrame(true);
      }
      break;
    case 'd':
      if(activeFrame == 0)
      {
        deleteClip();
        updateArrangerFrame(true);
      }
      break;
    case 'n':
      if(activeFrame == 0)
      {
        addCue();
        updateArrangerFrame(true);
      }
      break;
    case 't':
      if(activeFrame == 0)
      {
        addTrack();
        if(selectClip(tracks.size() - 1, currentClip))
        {
          updateDetailFrame(false);
        }
        updateArrangerFrame(true);
      }
      break;
    case 'q':
      quit = true;
      break;
    default:
      break;
  }
  if(quit)
    return -1;

  return ch;
}

int main(int argc, char *argv[]) {
  int ch = 0;
  startClock();
  engine = new audioEngine();
  std::thread worker = std::thread(workerLoop);
//  queue = new engineQueue();
  newFile();
  startcurses();
  initMainWindow();
  drawMainWindowHeader("NEW SONG");
  drawTempoHeader((int) globalTempo);
  initDetailFrame();
//  char initTrackName[] = "TRACK 0";
//  char initClipName[] = "CLIP 0";
//  sprintf(initTrackName, "TRACK %d", currentTrack+1);
//  sprintf(initClipName, "CLIP %d", currentClip+1);
//  drawDetailFrame(false, 0, tracks.at(currentTrack).name().c_str(), tracks.at(currentTrack).clips.at(currentClip).name().c_str());
  initArrangerFrame();
  updateArrangerFrame(true);
  updateDetailFrame(false);
//  drawArrangerFrame(true, tracks.size(), getTrackNames(), clipNames, currentTrack, currentClip);
//  sleep(5);
  while(true)
  {
//    doAllEvents();
    ch = parseKeys(handleKeys());
    if (ch < 0)
    {
      m_abort = true;
      break;
    }
  }
  worker.join();
  endcurses();
  return 0;
}

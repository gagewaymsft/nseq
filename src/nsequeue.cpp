#include "nsequeue.h"

long double startTime;
long double currentTime;
long double lastTime;
long double deltaTime;
std::vector<queueEvent> queue;

long double curTime()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(
    std::chrono::system_clock::now().time_since_epoch()
  ).count();
}

bool startClock()
{
  startTime = curTime();
  lastTime = startTime;
  currentTime = startTime;
//  startTime = (unsigned long) start;
  return true;
}

std::vector<queueEvent> doQueue(float tempo)
{
  float sixteenth = (60000 / tempo) / 4;
  std::vector<queueEvent> cQ;
  lastTime = currentTime;
  currentTime = curTime();
  deltaTime = (currentTime - lastTime) / sixteenth;

  for(int i = queue.size() - 1; i >= 0; i--)
  {
    queue.at(i).time -= deltaTime;
    if(queue.at(i).time <= 0)
    {
      cQ.push_back(queue.at(i));
//      queue.pop_back();
      queue.erase(queue.begin() + i);
    }
  }

  return cQ;
}

int enQueue(int type, int sample, float startTime, float endTime, int note, int velocity)
{
//  int time = (int) (startTime * 1000);
  if(type == -1)
  {
    queue.clear();
  }
  else
  {
    queueEvent newEvent = {startTime, type, sample, note, velocity};
    queue.push_back(newEvent);
  }
  return 0;
}

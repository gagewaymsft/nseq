#ifndef NSEQUEUE_H
#define NSEQUEUE_H

#include <chrono>
#include <vector>

struct queueEvent {
  float time;
  int type;
  int sample;
  int pitch;
  int velocity;
};

long double curTime();
bool startClock();
std::vector<queueEvent> doQueue(float tempo);
int enQueue(int type, int sample, float startTime, float endTime, int note, int velocity);

#endif

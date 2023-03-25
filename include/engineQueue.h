#ifndef ENGINEQUEUE_H
#define ENGINEQUEUE_H

class engineQueue
{
  public:
    engineQueue();
    ~engineQueue();
    void addEvent(int sample, int deltaTime, int note, int velocity);
    void doQueue();

};

#endif

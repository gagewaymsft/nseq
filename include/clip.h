#ifndef CLIP_H
#define CLIP_H
#include <string>
#include <vector>

struct note {
  float startTime;
  float endTime;
  int pitch;
  int velocity;
};

class clip
{
  int instrument;
  int _type;
  std::string _name;
  std::vector<note> _sequence;

  public:
    clip(int i, int t, std::string n);
    std::string name();
    int type();
    std::string typeString();
    void nextClipType();
    void setName(std::string n);
    std::string * getNamePointer();
    void addToSequence(float startTime, float endTime, int pitch, int velocity);
    std::vector<note> sequence();
};

#endif

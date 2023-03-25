#ifndef TRACK_H
#define TRACK_H

#include "clip.h"
#include <vector>

class track
{
  public:
    track(int clipCount, std::string n);
    std::vector<clip> clips;
    std::vector<std::string> getStringList(int maxSize);
    std::string typeString();
    int type();
    std::string name();
    void setName(std::string n);

  private:
    int _type = 0;
    std::string _name = "TRACK";
};

#endif

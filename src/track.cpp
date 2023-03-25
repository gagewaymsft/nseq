#include "track.h"

track::track(int clipCount, std::string n)
{
  clip configClip(0,0,"CONFIG");
  clips.push_back(configClip);
  clip emptyClip(0,-1,"none");
  clips.push_back(emptyClip);
  for(int i = 2; i < clipCount; i++)
  {
    clips.push_back(emptyClip);
  }

  _name = n;
}

std::vector<std::string> track::getStringList(int maxSize)
{
  std::vector<std::string> stringList;
  for(int i = 0; i < maxSize; i++)
  {
    if(i < clips.size())
      stringList.push_back(clips.at(i).name());
    else
      stringList.push_back("");
  }
  return stringList;
}

std::string track::typeString()
{
  std::string s = "";
  switch(_type)
  {
    case 0:
      s = "AUDIO";
      break;
    default:
      break;
  }
  return s;
}

int track::type()
{
  return _type;
}

std::string track::name()
{
  return _name;
}

void track::setName(std::string n)
{
  _name = n;
}

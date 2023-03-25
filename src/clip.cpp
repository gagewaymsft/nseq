#include "clip.h"

clip::clip(int i, int t, std::string n)
{
  instrument = i;
  _name = n;
  _type = t;
  addToSequence(0, 10, 45, 127);
  addToSequence(0, 10, 69, 127);
  addToSequence(2, 10, 57, 127);
  addToSequence(4, 10, 45, 127);
  addToSequence(6, 10, 57, 127);
  addToSequence(8, 10, 45, 127);
  addToSequence(3, 10, 69, 127);
  addToSequence(10, 10, 57, 127);
  addToSequence(12, 10, 45, 127);
  addToSequence(14, 10, 57, 127);
  addToSequence(16, 10, 45, 127);
  addToSequence(16, 10, 67, 127);
  addToSequence(18, 10, 57, 127);
  addToSequence(19, 10, 67, 127);
  addToSequence(20, 10, 45, 127);
  addToSequence(22, 10, 57, 127);
  addToSequence(24, 10, 45, 127);
  addToSequence(24, 10, 65, 127);
  addToSequence(26, 10, 57, 127);
  addToSequence(27, 10, 65, 127);
  addToSequence(28, 10, 45, 127);
  addToSequence(30, 10, 57, 127);
}

std::string clip::name()
{
  return _name;
}

int clip::type()
{
  return _type;
}

std::string clip::typeString()
{
  std::string str = "";
  switch(_type)
  {
    case(-1):
      str = "none";
      break;
    case(0):
      str = "CONFIG";
      break;
    case(1):
      str = "SAMPLER";
      break;
    case(2):
      str = "FADE";
      break;
    case(3):
      str = "SEQUENCER";
      break;
    default:
      break;
  }
  return str;
}

void clip::nextClipType()
{
  if(_type <= 0)
    return;

  _type++;
  if(_type > 3)
    _type = 1;
}

void clip::setName(std::string n)
{
  _name = n;
}

/*
std::string * clip::getNamePointer()
{
  return * _name;
}
*/

void clip::addToSequence(float startTime, float endTime, int pitch, int velocity)
{
  note newNote = {startTime, endTime, pitch, velocity};
  _sequence.push_back(newNote);
}

std::vector<note> clip::sequence()
{
  return _sequence;
}

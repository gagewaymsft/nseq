#include "sequencer.h"
#include "clip.h"
#include <vector>

std::vector<note> currentSequence;
//extern float xSeqScale = 1;
//extern float ySeqScale = 1;

void addNoteToSequence(float startTime, float endTime, int pitch, int velocity)
{
  note newNote = {startTime, endTime, pitch, velocity};
  currentSequence.push_back(newNote);
}

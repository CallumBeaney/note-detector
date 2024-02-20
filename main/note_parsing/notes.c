#include "notes.h"

static Note nearestNoteToFrequency(int foi, Note* MusicalNotes, int arrLen);


const char* note_parseNote(int frequency) 
{
  int musicalNotesLen = (sizeof(MusicalNotes) / sizeof(MusicalNotes[0]));

  int thisNote = frequency;
  if (thisNote < MusicalNotes[0].frequency || thisNote > MusicalNotes[musicalNotesLen].frequency) return NULL;

  int buffLen = 100;
  char* buffer = malloc(sizeof(char) * buffLen);


  Note nearestNote = nearestNoteToFrequency(thisNote, MusicalNotes, musicalNotesLen);

  int difference = (int)fabs(thisNote - nearestNote.frequency);
  bool greaterOrLower = thisNote > nearestNote.frequency ? true : false; 

  snprintf(buffer, buffLen, "%d : %s ( %s%d Hz)\n", frequency, nearestNote.name, greaterOrLower ? "+" : "-", difference);
  return buffer;
}


static Note nearestNoteToFrequency(int foi, Note* notes, int arrLen) 
{   
  Note nearestFreq = {0, NULL};
  for (int i = 0; i < arrLen; i++) {
    if (notes[i].frequency == (float) foi) return notes[i];
    if (fabs(foi - notes[i].frequency)  <  fabs(foi - nearestFreq.frequency)) {
      nearestFreq = notes[i];
    }
  }
  return nearestFreq;
}

#include "notes.h"
#include "esp_log.h"


static Note nearestNoteToFrequency(int foi, Note* Notes, int arrLen);

const char* TAG = "NOTES";


void note_parseNote(char* buffer, int buffLen, int frequency) 
{
  ESP_LOGI(TAG, "MAX FREQ: %f", MUSICAL_NOTES[MUSICAL_NOTES_LEN].frequency);

  int thisNote = frequency;

  if (thisNote < MUSICAL_NOTES[0].frequency) {
    snprintf(buffer, buffLen, "NO FREQUENCY DETECTED");
    return;
  }

  int maxFreq = MUSICAL_NOTES[MUSICAL_NOTES_LEN].frequency;

  if (thisNote > maxFreq) {
    snprintf(buffer, buffLen, "MAX FREQUENCY: %d", maxFreq);
    return;
  }

  Note note = nearestNoteToFrequency(thisNote, MUSICAL_NOTES, MUSICAL_NOTES_LEN);
  if (note.name == NULL || note.frequency == 0) {
    snprintf(buffer, buffLen, "NO FREQUENCY DETECTED");
    ESP_LOGE(TAG, "nearestNoteToFrequency returned empty Note struct");
  }

  int difference = (int)fabs(thisNote - note.frequency);
  bool greaterOrLower = thisNote > note.frequency ? true : false; 
  
  snprintf(buffer, buffLen, "%d : %s ( %s%d Hz)\n", frequency, note.name, greaterOrLower ? "+" : "-", difference);

  return;
}


static Note nearestNoteToFrequency(int foi, Note* notes, int notesLen) 
{   
  Note nearestFreq = {0, NULL};
  for (int i = 0; i < notesLen; i++) {
    if (notes[i].frequency == (float) foi) return notes[i];
    if (fabs(foi - notes[i].frequency)  <  fabs(foi - nearestFreq.frequency)) {
      nearestFreq = notes[i];
    }
  }
  return nearestFreq;
}

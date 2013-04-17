#include <core.h>
#include "pitches.h"

#define TONE_PIN 5 // only pin 5 and 6 support tone function
                  
// notes in the melody:
int melody[] = {
   NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
   4, 8, 8, 4, 4, 4, 4, 4 };

void setup() 
{
   int noteDuration = 0;
   int thisNote = 0;
   int pauseBetweenNotes = 0;

   // iterate over the notes of the melody:
   for (thisNote = 0; thisNote < 8; thisNote++) {

      // to calculate the note duration, take one second 
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      noteDuration = 1000/noteDurations[thisNote];
      tone(TONE_PIN, melody[thisNote], noteDuration);

      //pauseBetweenNotes = noteDuration * 1.30;
      delay(noteDuration);
      // stop the tone playing:
      noTone(TONE_PIN);
   }
}

void loop()
{
   tone(TONE_PIN, 440, 200);
   delay(200);

   tone(TONE_PIN, 494, 500);
   delay(200);

   tone(TONE_PIN, 523, 300);
   delay(200);
}

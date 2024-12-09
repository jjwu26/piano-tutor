/* CICS256: Make
 * Final Project: Piano Tutor
 */

#include <Arduino.h>
#define PIN 2 
// do we have to define each pin?

const int piezoPin = 8; // pin connected to the buzzer
const int buttonPins[] = {2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, A0}; // sample pins

// note frequencies for one octave
const int notes[] = {
  262, // C4
  277, // C#4
  294, // D4
  311, // D#4
  330, // E4
  349, // F4
  370, // F#4
  392, // G4
  415, // G#4
  440, // A4
  466, // A#4
  494  // B4
};

void setup() { //might have to change this to configure LEDs
  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  pinMode(piezoPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < 12; i++) {
    // check if the button is pressed 
    if (digitalRead(buttonPins[i]) == LOW) {
      tone(piezoPin, notes[i]); // play the corresponding note
      delay(200);              // delay for note duration
      noTone(piezoPin);        // stop playing
    }
  }
}

//not sure how to do pauses lol       
//twinkle twinkle: [0, 0, 7, 7, 9, 9, 7, 5, 5, 4, 4, 2, 2, 0]
//duration [200, 200, 200, 200, 200, 200, 300, 200, 200, 200, 200, 200, 200, 300]

void game(int[] song) {
  //play LED instructions
  //change pins to output mode
  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], OUTPUT);
  }
  
  for (int i = 0; i < song.length; i++){
    // play the corresponding tone
      tone(piezoPin, notes[song[i]]);  
    // light up the right LED
      digitalWrite(buttonPins[song[i]], 1); 
      delay(200);
      digitalWrite(buttonPins[song[i]], 0);
  }

  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  
  for (int i = 0; i < song.length; i++){
      int correctNote = song[i];
      for(int j = 0; j < 12; j++){
          if (digitalRead(buttonPins[j]) == LOW && j == i) {
              tone(piezoPin, notes[j]); // play the corresponding note
              delay(200);              // delay for note duration
              noTone(piezoPin);        // stop playing
          } else if (digitalRead(buttonPins[j]) == LOW && j != i){
              //they played the wrong note
          }
      }
  }
    // light up the right LED
      digitalWrite(buttonPins[song[i]], 1); 
      delay(200);
      digitalWrite(buttonPins[song[i]], 0);
  }
  //track button presses and alert when incorrect
}

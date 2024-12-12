/* CICS256: Make
 * Final Project: Piano Tutor
 */
#include <SSD1306Wire.h>
#include <Arduino.h>

#define LED 22
SSD1306Wire display(0x3c, SDA, SCL);

// have to define each pin prob
const int piezoPin = 25; // whatever pin is connected to the buzzer
const int buttonPins[] = {34, 35, 32, 33, 4, 26, 27, 14, 12, 13, 15, 2}; // pin list

//twinkle twinkle
int song[] = {0, 0, 7, 7, 9, 9, 7, 5, 5, 4, 4, 2, 2, 0};
//jingle bells
int song2[] = {4, 4, 4, 4, 4, 4, 4, 6, 0, 2, 4};

int duration[] = {200, 200, 200, 200, 200, 200, 300, 200, 200, 200, 200, 200, 200, 300};
int duration2[] = {200, 200, 300, 200, 200, 300, 200, 200, 200, 200, 300};

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
  pinMode(LED, OUTPUT);
  pinMode(piezoPin, OUTPUT);

  ledcSetup(0, 5000, 8);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_24);
  display.display();  

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
    } else{
      noTone(piezoPin);        // stop playing
    }
  }
}

// play LED instructions and correct tune
void play_song(int song[], int duration[], int length){
  //change pins to output mode
  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], OUTPUT);
  }
  for (int i = 0; i < length; i++){
    // play the corresponding tone
      tone(piezoPin, notes[song[i]]);  
    // light up the right LED
      digitalWrite(buttonPins[song[i]], 1); 
      delay(duration[i]);
      digitalWrite(buttonPins[song[i]], 0);
      noTone(piezoPin);        // stop playing
  }
  for (int i = 0; i < 12; i++) {
       pinMode(buttonPins[i], INPUT_PULLUP);
  }
  
}


void game(int song[], int duration[], int song_length) {
  play_song(song, duration, song_length);
  
  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  
  //track button presses and alert when incorrect
  for (int i = 0; i < sizeof(song)/sizeof(song[i]); i++){
      int correctNote = song[i];
      boolean clicked = false;
      while(!clicked){
        for(int j = 0; j < 12; j++){
            if (digitalRead(buttonPins[j]) == LOW && j == i) {
                clicked = true;
                tone(piezoPin, notes[j]); // play the corresponding note
                while(digitalRead(buttonPins[j]) == LOW ){
                  //keep playing sound until the key is released
                }
                noTone(piezoPin);        // stop playing
            } else if (digitalRead(buttonPins[j]) == LOW && j != i){
                clicked = true;
                //they played the wrong note so light up the right one
                digitalWrite(buttonPins[j],1);
                while(digitalRead(buttonPins[j]) == HIGH ){
                  //wait until key is pressed 
                }
                digitalWrite(buttonPins[correctNote],0);
                tone(piezoPin, notes[j]); // play the corresponding note
                while(digitalRead(buttonPins[j]) == LOW ){
                  //keep playing until the key is released
                }
            }
        }
      }
  }

}


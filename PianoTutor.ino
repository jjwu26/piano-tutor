/* CICS256: Make
 * Final Project: Piano Tutor
 */
#include <SSD1306Wire.h>
#include <Arduino.h>

#define LCD 22

// have to define each pin prob
const int piezoPin = 25; // whatever pin is connected to the buzzer
const int buttonPins[] = {34, 35, 32, 33, 4, 26, 27, 14, 12, 13, 15, 2}; // pin list

// twinkle twinkle
int song[] = {0, 0, 7, 7, 9, 9, 7, 5, 5, 4, 4, 2, 2, 0};
int duration[] = {400, 400, 400, 400, 400, 400, 500, 400, 400, 400, 400, 400, 400, 500};
int length = 14;

// jingle bells
int song2[] = {4, 4, 4, 4, 4, 4, 4, 6, 0, 2, 4};
int duration2[] = {200, 200, 300, 200, 200, 300, 200, 200, 200, 200, 300};
int length2 = 11;

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
  pinMode(piezoPin, OUTPUT);
  ledcSetup(0, 5000, 8);

  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  Serial.print("Setup completed");

  //play_song(song, duration, length);
}


void loop() {
  for (int i = 0; i < 12; i++) {
    // check if the button is pressed 
    if (digitalRead(buttonPins[i]) == LOW) {
      tone(piezoPin, notes[i]); // play the corresponding note
      Serial.print("Button pressed: ");
      Serial.println(i);
      delay(300);
      noTone(piezoPin);        // stop playing
    }
  }

  int chosen_song = song_selection();
  if(chosen_song == 1){
    game(song, duration, length);
  } else if(chosen_song == 2){
    game(song2, duration2, length2);
  }
}

// play LED instructions and correct tune
void play_song(int song[], int duration[], int length){
  //change pins to output mode
  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  for (int i = 0; i < length; i++){
    // light up the right LED
      pinMode(buttonPins[i], OUTPUT);
    // play the corresponding tone
      tone(piezoPin, notes[song[i]]);  
      delay(duration[i]);
      pinMode(buttonPins[i], INPUT_PULLUP);

  }
  // for (int i = 0; i < 12; i++) {
  //      pinMode(buttonPins[i], INPUT_PULLUP);
  // }
}

void play_mod_tone(int freq, int duration){
  int time = millis();
  while(time < millis()+duration){
    float currentAngle = 0;
    float dutyCycle = map(sin(currentAngle), -1, 1, 10, 90); // Adjust values to control the rounding effect
    tone(piezoPin, freq, dutyCycle);
    currentAngle += 0.1; // Increment the angle for the next loop
  }
  noTone(piezoPin);        // stop playing
}

void game(int song[], int duration[], int song_length) {

  play_song(song, duration, song_length);

  //ensuring buttons can recieve input  
  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT);
  }
  
  //number of correct notes played
  int score = 0;

  //track button presses and alert when incorrect
  for (int i = 0; i < song_length; i++){
      int correctNote = song[i];
      boolean clicked = false;
      while(!clicked){
        for(int j = 0; j < 12; j++){
            if (digitalRead(buttonPins[j]) == LOW && j == i) {
                clicked = true;
                tone(piezoPin, notes[j]); // play the corresponding note
                // while(digitalRead(buttonPins[j]) == LOW ){
                //   //keep playing sound until the key is released
                // }
                noTone(piezoPin);        // stop playing
                score ++;
            } else if (digitalRead(buttonPins[j]) == LOW && j != i){
                //they played the wrong note so light up the right one
                digitalWrite(buttonPins[j],1);
                digitalWrite(buttonPins[correctNote],0);
                tone(piezoPin, notes[j]); // play the corresponding note
                clicked = true;
            }
        }
      }
  }
  Serial.print("final score: ");
  Serial.println(score);
}

int song_selection(){
  //let user pick which song they want to learn
  Serial.println("Select a song to learn by pressing the appropriate key!");
  Serial.println("C: Twinkle Twinkle Little Star");
  Serial.println("D: Jingle Bells");
  Serial.println("E: ");
  Serial.println("F: ");
  Serial.println("G: ");
  
  while (true){
    for (int i = 0; i < 12; i++) {
        // check if the button is pressed 
        if (digitalRead(buttonPins[i]) == LOW) {
              return i;    
        }
    }
  }
  


}


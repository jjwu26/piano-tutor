/* CICS256: Make
 * Final Project: Piano Tutor
 */
#include <SSD1306Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


const int piezoPin = 25; 
const int buttonPins[] = {34, 35, 32, 33, 4, 26, 27, 14, 12, 13, 15, 2}; // pin list

LiquidCrystal_I2C lcd(0x27, 16, 2);

// twinkle twinkle
int song[] = {0, 0, 7, 7, 9, 9, 7, 5, 5, 4, 4, 2, 2, 0};
int duration[] = {400, 400, 400, 400, 400, 400, 500, 400, 400, 400, 400, 400, 400, 500};
int length = 14;

// jingle bells
int song2[] = {4, 4, 4, 0, 4, 4, 4, 0, 4, 7, 2, 3, 4};
int duration2[] = {400, 400, 800, 400, 400, 800, 400, 400, 400, 400, 400, 400, 800};
int length2 = 13;

//ode to joy
int song5[] = {4, 4, 5, 7, 7, 5, 4, 2, 0, 0, 2, 4, 4, 2, 2, 
               4, 4, 5, 7, 7, 5, 4, 2, 0, 0, 2, 4, 2, 0, 0};
int duration5[] = {400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 600, 200, 800,
                   400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 600, 200, 800};
int length5 = 30;

// Happy Birthday
int song4[] = {0, 0, 2, 0, 5, 4, 0, 0, 2, 0, 7, 5, 0, 0, 12, 9, 5, 4, 2, 10, 10, 9, 5, 7, 5};
int duration4[] = {400, 400, 500, 500, 500, 500, 500, 400, 400, 500, 500, 500, 400, 400, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
int length4 = 25;

int star_wars[] = {4, 4, 4, 6, 11, 4, 6, 11, 4};
int star_wars_duration[] = {
    300, 300, 300, 
    500, 
    300, 
    300, 500, 
    800
};


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

void setup() { 
  pinMode(piezoPin, OUTPUT);

  for (int i = 0; i < 12; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(1, 0);

  game_setup();
}

void loop() {
  // for (int i = 0; i < 12; i++) {
  //   // check if the button is pressed 
  //   if (digitalRead(buttonPins[i]) == LOW) {
  //     // play_mod_tone(notes[i], 100);
  //     tone(piezoPin, notes[i]); // play the corresponding note
  //     delay(300);
  //     noTone(piezoPin);        // stop playing
  //   }
  // }
  
}


void game_setup(){
  lcd.print("Welcome to Piano Tutor");
  delay(1000);
  lcd.clear();

  int chosen_song = song_selection();
  delay(1000);

  lcd.clear();
  lcd.print(chosen_song);
  if(chosen_song == 0){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Twinkle Twinkle");
    delay(1000);
    game(song, duration, length);
  } else if(chosen_song == 2){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Jingle Bells");
    delay(1000);
    game(song2, duration2, length2);
  } else if(chosen_song == 4){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Star Wars");
    delay(1000);
    game(star_wars, star_wars_duration, 9);
  }else if(chosen_song == 5){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Happy Birthday");
    delay(1000);
    game(song4, duration4, 25);
  } else if(chosen_song == 7){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Ode to Joy");
    delay(1000);
    game(song5, duration5, 30);
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
      pinMode(buttonPins[song[i]], OUTPUT);
    // play the corresponding tone
      tone(piezoPin, notes[song[i]]);  
      delay(duration[i]);
      // play_mod_tone(notes[song[i]], duration[i]);
      pinMode(buttonPins[song[i]], INPUT_PULLUP);
  }
  noTone(piezoPin);

  
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
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print(getRightKey(correctNote));
      delay(500);
      boolean clicked = false;
      while(!clicked){
        for(int j = 0; j < 12; j++){
            if (digitalRead(buttonPins[j]) == LOW){
                // lcd.print(getRightKey(j));
                if(j == correctNote){
                  clicked = true;
                  tone(piezoPin, notes[correctNote]); // play the corresponding note
                  delay(300);
                  noTone(piezoPin);        // stop playing
                  score ++; 
                } else {
                  if(j != 11){ // unfortunately there is a bug with the B key so i made this check to ignore it
                    lcd.setCursor(0, 1);
                    lcd.print("Wrong note!");
                    score --;
                    digitalWrite(buttonPins[correctNote],1);
                    delay(1000);
                    digitalWrite(buttonPins[correctNote],0);
                  }
                }
            }
              
        }
      }
  }
  lcd.clear();
  lcd.print("Score: ");
  lcd.print(score);
  lcd.print("/");
  lcd.print(song_length);
  delay(3000);
  lcd.clear();
  lcd.print("Press C to play again");
  while (true){
        if (digitalRead(buttonPins[0]) == LOW) {
            game_setup();
        }
  }
  
}

String getRightKey(int key){
  if(key == 0){
    return "C";
  } else if (key == 1){
    return "C#";
  } else if (key == 2){
    return "D";
  } else if (key == 3){
    return "D#";
  } else if (key == 4){
    return "E";
  } else if (key == 5){
    return "F";
  } else if (key == 6){
    return "F#";
  } else if (key == 7){
    return "G";
  } else if (key == 8){
    return "G#";
  } else if (key == 9){
    return "A";
  } else if (key == 10){
    return "A#";
  } else if (key == 11){
    return "B";
  }
}

int song_selection(){
  //let user pick which song they want to learn
  lcd.print("Select a song");
  delay(2000);
  lcd.clear();

  lcd.print("to begin learning");
  delay(2000);
  lcd.clear();

  lcd.print("by pressing the");
  delay(2000);
      lcd.clear();


  lcd.print(" appropriate key:");
  delay(2000);
      lcd.clear();


  lcd.print("C: Twinkle Twinkle");
    delay(2000);
    lcd.clear();


  lcd.print("D: Jingle Bells");
    delay(2000);
    lcd.clear();

  lcd.print("E: Star Wars");
    delay(2000);
    lcd.clear();

  lcd.print("F: Happy Birthday");
    delay(2000);
    lcd.clear();

  lcd.print("G: Ode to Joy");
    delay(2000);
      lcd.clear();

  lcd.print("Please press a key. ");
  while (true){
    for (int i = 0; i < 12; i++) {
        // check if the button is pressed 
        if (digitalRead(buttonPins[i]) == LOW) {
              lcd.clear();
            lcd.print("Initializing game. ");
              return i;    
        }
    }
  }

}


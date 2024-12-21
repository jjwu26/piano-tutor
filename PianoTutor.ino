/* CICS256: Make
 * Final Project: Piano Tutor
 */
#include <SSD1306Wire.h>
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>


const int piezoPin = 25; 
const int buttonPins[] = {34, 35, 32, 33, 4, 26, 27, 14, 12, 13, 15, 2}; // pin list
//pin [11] / 2 may be shorted

LiquidCrystal_I2C lcd(0x27, 16, 2);

// twinkle twinkle
int song_twinkle[] = {0, 0, 7, 7, 9, 9, 7, 5, 5, 4, 4, 2, 2, 0};
int duration_twinkle[] = {400, 400, 400, 400, 400, 400, 500, 400, 400, 400, 400, 400, 400, 500};
int length_twinkle = 14;

int song_jingle[] = {4, 4, 4, 4, 4, 4, 4, 7, 0, 2, 4};
int duration_jingle[] = {400, 400, 800, 400, 400, 800, 400, 400, 400, 400, 800};
int length_jingle = 11;

//ode to joy
int song_joy[] = {4, 4, 5, 7, 7, 5, 4, 2, 0, 0, 2, 4, 4, 2, 2, 4, 4, 5, 7, 7, 5, 4, 2, 0, 0, 2, 4, 2, 0, 0};
int duration_joy[] = {400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 600, 200, 800, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 400, 600, 200, 800};
int length_joy = 30;

// Happy Birthday
int song_bday[] = {0, 0, 2, 0, 5, 4, 0, 0, 2, 0, 7, 5, 0, 0, 11, 9, 5, 4, 2, 10, 10, 9, 5, 7, 5};
int duration_bday[] = {400, 400, 500, 500, 500, 500, 500, 400, 400, 500, 500, 500, 400, 400, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500};
int length_bday = 25;

//star wars
int star_wars[] = {4, 4, 4, 6, 11, 4, 6, 11, 4};
int star_wars_duration[] = {300, 300, 300, 500, 300, 300, 500, 800};
int star_wars_length = 9;

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
void printCenteredTwoRows(const String &text) {
    // Find the last space before the midpoint
    int mid = text.length() / 2;
    int lastSpace = text.lastIndexOf(' ', mid);

    // If no space is found, use the midpoint
    if (lastSpace == -1) {
        lastSpace = mid; // Fallback to mid if no space is found
    }

    // Split the text into two parts
    String firstPart = text.substring(0, lastSpace);
    String secondPart = text.substring(lastSpace + 1); // Skip the space

    // Center the first part on row 0
    int len1 = firstPart.length();
    int pos1 = (16 - len1) / 2;
    if (pos1 < 0) pos1 = 0; // Ensure the position is not negative
    lcd.setCursor(pos1, 0); // Set cursor to the calculated position for row 0
    lcd.print(firstPart); // Print the first part

    // Center the second part on row 1
    int len2 = secondPart.length();
    int pos2 = (16 - len2) / 2;
    if (pos2 < 0) pos2 = 0; // Ensure the position is not negative
    lcd.setCursor(pos2, 1); // Set cursor to the calculated position for row 1
    lcd.print(secondPart); // Print the second part
}

void printCentered(const String &text) {
    int len = text.length(); // Get the length of the text
    int pos = (16 - len) / 2; // Calculate the starting position for centering
    if (pos < 0) pos = 0; // Ensure the position is not negative
    lcd.setCursor(pos, 0); // Set cursor to the calculated position
    lcd.print(text); // Print the text
}

void setup() { 
    pinMode(piezoPin, OUTPUT);
    for (int i = 0; i < 12; i++) {
        pinMode(buttonPins[i], INPUT_PULLUP);
    }
    lcd.init();
    lcd.backlight();

    printCenteredTwoRows("Welcome to Piano Tutor!");
    delay(5000);
    lcd.clear();
    
    printCenteredTwoRows("Select a song to begin learning.");
    delay(5000);
    lcd.clear();

    printCenteredTwoRows("Choose the appropriate key.");
    delay(5000);
    lcd.clear();

    game_setup();
}

void loop() {
    //not needed
}

void game_setup(){

    int chosen_song = song_selection();
    delay(1000);

    lcd.clear();
    lcd.print(chosen_song);

  if (chosen_song == 0){
    lcd.clear();
    printCenteredTwoRows("Twinkle Twinkle Lil Star");
    delay(1000);
    game(song_twinkle, duration_twinkle, length_twinkle);
  } else if (chosen_song == 2){
    lcd.clear();
    printCentered("Jingle Bells");
    delay(1000);
    game(song_jingle, duration_jingle, length_jingle);
  } else if (chosen_song == 4){
    lcd.clear();
    printCentered("Star Wars");
    delay(1000);
    game(star_wars, star_wars_duration, star_wars_length);
  } else if (chosen_song == 5){
    lcd.clear();
    printCentered("Happy Birthday");
    delay(1000);
    game(song_bday, duration_bday, length_bday);
  } else if (chosen_song == 7){
    lcd.clear();
    printCentered("Ode to Joy");
    delay(1000);
    game(song_joy, duration_joy, length_joy);
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
        //digitalWrite(buttonPins[song[i]], HIGH);

        // play the corresponding tone
        tone(piezoPin, notes[song[i]]);  

        //play_mod_tone(notes[song[i]], duration[i]);

        delay(duration[i]);
        //digitalWrite(buttonPins[song[i]], LOW); 
        pinMode(buttonPins[song[i]], INPUT_PULLUP);
    }
    noTone(piezoPin);
}

void play_mod_tone(int freq, int duration){
    int time = millis();
    while(time < millis()+duration){
        float currentAngle = 0;
        float dutyCycle = map(sin(currentAngle), -1, 1, 10, 90); // Adjust values to control the rounding effect
        tone(piezoPin, freq, dutyCycle);
        currentAngle += 0.1; // Increment the angle for the next loop
    }
    noTone(piezoPin); // stop playing
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
                    if (j == correctNote) {
                        clicked = true;
                        tone(piezoPin, notes[correctNote]); // play the corresponding note
                        delay(300);
                        noTone(piezoPin);        // stop playing
                        score ++; 
                    } 
                    else {
                        if(j != 11){ // unfortunately there is a bug with the B key so i made this check to ignore it
                            lcd.setCursor(0, 1);
                            lcd.print("Wrong note!");
                            score--;
                            digitalWrite(buttonPins[correctNote], 1);
                            delay(1000);
                            digitalWrite(buttonPins[correctNote], 0);
                        }
                    }
                }
            }
        }
    }
    // does the percentage score
    int percentage = (score * 100) / song_length; 
    String message; 
    if (percentage == 100) {
        message = "Awesome job!";
    } else if (percentage >= 80) {
        message = "Good Effort!";
    } else {
        message = "Try again!";
    }
    lcd.clear();

    lcd.setCursor(0, 0);
    lcd.print("Score: " + String(percentage) + "%"); 
    lcd.setCursor(0, 1); 
    lcd.print(message);
    delay(5000); 
    lcd.clear();

    printCenteredTwoRows("Press C to play again");

    bool waitingForInput = true; 
    while (waitingForInput) {
        if (digitalRead(buttonPins[0]) == LOW) {
            waitingForInput = false; 
        }
    }
    game_setup();
  
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
void scrollText(const String &text) {
    int len = text.length();
    for (int i = 0; i < len + 16; i++) { 
        lcd.clear();
        if (i < 16) {
            lcd.print(text.substring(0, i)); 
        } else {
            lcd.print(text.substring(i - 16, i));
        }
        delay(100); 
    }
}

int song_selection(){
  //let user pick which song they want to learn
    printCenteredTwoRows("C: Twinkle Twinkle Lil Star");
    delay(3000);
    lcd.clear();

    printCenteredTwoRows("D: Jingle Bells");
    delay(3000);
    lcd.clear();

    printCentered("E: Star Wars");
    delay(3000);
    lcd.clear();

    printCenteredTwoRows("F: Happy Birthday to You");
    delay(3000);
    lcd.clear();

    printCentered("G: Ode to Joy");
    delay(3000);
    lcd.clear();

    printCenteredTwoRows("Choose a key: C, D, E, F, G");

    while (true) {
        if (digitalRead(buttonPins[0]) == LOW) { // C
            lcd.clear();
            scrollText("Initializing the game");
            delay(500); // Debounce delay
            return 0; // Return index for C
        }
        if (digitalRead(buttonPins[2]) == LOW) { // D
            lcd.clear();
            scrollText("Initializing the game");
            delay(500); // Debounce delay
            return 2; // Return index for D
        }
        if (digitalRead(buttonPins[4]) == LOW) { // E
            lcd.clear();
            scrollText("Initializing the game");
            delay(500); // Debounce delay
            return 4; // Return index for E
        }
        if (digitalRead(buttonPins[5]) == LOW) { // F
            lcd.clear();
            scrollText("Initializing the game");
            delay(500); // Debounce delay
            return 5; // Return index for F
        }
        if (digitalRead(buttonPins[7]) == LOW) { // G
            lcd.clear();
            scrollText("Initializing the game");
            delay(500); // Debounce delay
            return 7; // Return index for G
        }
    }
}
/*
  This sketch demonstrates how the raw output of the scan fonction can be used.
  Just hook up a small speaker to pin 10 and you can play notes with the keypad.
*/

#include <SimpleKeypad.h>

#define speaker_pin 10

const byte nb_rows = 4;                         // four rows
const byte nb_cols = 4;                         // four columns
char key_chars[nb_rows][nb_cols] = {            // Numbers used as indexes of the notes list
  {1, 2, 3, 4},                                 // (not 0 because it is the 'no key' value)
  {5, 6, 7, 8},
  {9, 10, 11, 12},
  {13, 14, 15, 16}
};
byte rowPins[nb_rows] = {9, 8, 7, 6};           // The pins where the rows are connected
byte colPins[nb_cols] = {5, 4, 3, 2};           // The pins where the columns are connected

SimpleKeypad kp1((char*)key_chars, rowPins, colPins, nb_rows, nb_cols);   // New keypad called kp1

int notes[] = {                                 // Frenquencies
  262, 294, 330, 349, 392, 440, 494,
  525, 587, 659, 698, 784, 880, 988,
  1047, 1175
};

void setup() {
}

void loop() {
  char key = kp1.scan();                      // The scan function scans the keypad and returns the key currently being pressed
  if (key) {
    tone(speaker_pin, notes[key - 1]);
  } else {
    noTone(speaker_pin);
  }
}

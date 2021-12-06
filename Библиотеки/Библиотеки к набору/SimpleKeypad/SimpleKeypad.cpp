#include "SimpleKeypad.h"

SimpleKeypad::SimpleKeypad(char *key_chars, byte *row_pins, byte *col_pins, byte nb_rows, byte nb_cols) {
  this->key_chars = key_chars;
  this->row_pins = row_pins;
  this->col_pins = col_pins;
  this->nb_rows = nb_rows;
  this->nb_cols = nb_cols;

  for (byte r = 0; r < nb_rows; r++) {
    pinMode(row_pins[r], INPUT_PULLUP);
  }
  for (byte c = 0; c < nb_cols; c++) {
    digitalWrite(col_pins[c], HIGH);
    pinMode(col_pins[c], INPUT);
  }
}

char SimpleKeypad::scan() {
  char result = '\0';
  for (byte c = 0; c < nb_cols; c++) {
    pinMode(col_pins[c], OUTPUT);               // Each column is set to a low output one after the other
    digitalWrite(col_pins[c], LOW);
    for (byte r = 0; r < nb_rows; r++) {
      if (!digitalRead(row_pins[r])) {          // For each column we can then test if any line is low, which would mean
        result = key_chars[r * nb_cols + c];    // that the key at these speciic row and column is being pressed.
      }
    }
    digitalWrite(col_pins[c], HIGH);
    pinMode(col_pins[c], INPUT);                // This ensures that only one pin is an output at a time, to prevent low and high output
  }                                             // from being shorted together if several keys are pressed at the same time
  return result;
}

char SimpleKeypad::getKey() {
  if (millis() - timer > debounce) {            // To prevent slowing down the code too much, the scan function is only executed every
    timer = millis();                           // 10 ms. This value can be changed in the header file SimpleFeypad.h
    char key = scan();
    if (key && !hold) {
      hold = 1;                                 // This function does the same thing as scan except it only returns the key one time
      return key;
    } else if (!key) {
      hold = 0;
    }
  }
  return '\0';
}

#ifndef simple_keypad_lib
#define simple_keypad_lib

#include <Arduino.h>

class SimpleKeypad {
  public:
    SimpleKeypad(char *key_charsA, byte *row_pinsA, byte *col_pinsA, byte nb_rowsA, byte nb_colsA);
    char scan();
    char getKey();

  private:
    char *key_chars;
    byte *row_pins;
    byte *col_pins;
    byte nb_rows;
    byte nb_cols;
    static const int debounce = 10;
    unsigned long timer = 0;
    bool hold = 0;
};

#endif

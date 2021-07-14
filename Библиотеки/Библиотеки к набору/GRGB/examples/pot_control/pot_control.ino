// управляем крутилкой разные режимы

#include <GRGB.h>
GRGB led(COMMON_CATHODE, 9, 10, 11);

void setup() {
}

void loop() {
  int val = analogRead(0);

  // цвета 8 бит
  led.setWheel8(val / 4); // 0-255
  
  /*
    // kelvin
    int K = map(val, 0, 1023, 1000, 10000);
    led.setKelvinFast(K);
  */

  delay(10);
}

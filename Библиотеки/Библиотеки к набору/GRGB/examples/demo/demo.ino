#include <GRGB.h>
// указать тип драйвера COMMON_CATHODE/COMMON_ANODE
// и пины в порядке R,G,B
GRGB led(COMMON_CATHODE, 9, 10, 11);

void setup() {
  // установить ОБЩУЮ яркость
  led.setBrightness(255);

  // включить гамма-коррекцию (по умолч. выключено)
  led.setCRT(true);
  //led.setCRT(false);  // выключить

  // установить цвет RGB [0-255]
  led.setRGB(255, 0, 0);
  //led.setRGB(255, 0, 0, 120); // + "локальная" яркость [0-255]
  delay(1000);

  // установить цвет HSV (цвет, насыщенность, яркость) [0-255]
  led.setHSV(120, 200, 250);
  delay(1000);

  // установить цвет HSV быстрый алгоритм (цвет, насыщенность, яркость) [0-255]
  led.setHSVfast(120, 200, 250);
  delay(1000);

  // установить цвет из цветового колеса [0-1530]
  led.setWheel(800);
  //led.setWheel(800, 120); // + "локальная" яркость [0-255]
  delay(1000);

  // установить цвет из цветового колеса [0-255]
  led.setWheel8(120);
  //led.setWheel8(120, 255); // + "локальная" яркость [0-255]
  delay(1000);

  // установить цветовую температуру [1000-40000] Кельвинов
  led.setKelvin(5500);
  //led.setKelvin(5500, 255); // + "локальная" яркость [0-255]
  delay(1000);

  // установить HEX цвет (24 бита)
  led.setHEX(0x00FF00);
  //led.setHEX(0x00FF00, 120); // + "локальная" яркость [0-255]
  delay(1000);

  // установить HEX цвет (16 бит)
  led.setHEX16(0xF81F);
  //led.setHEX16(0xF81F, 120); // + "локальная" яркость [0-255]
  delay(1000);

  // установить предустановленный цвет (16 бит)
  led.setColor(GAqua);  // список цветов в GRGB.h
  //led.setColor(GAqua, 120); // + "локальная" яркость [0-255]
  delay(1000);
}

void loop() {
  // плавно мигаем яркостью
  static uint32_t tmr;
  if (millis() - tmr >= 5) {
    tmr = millis();
    static int8_t dir = 1;
    static int val = 0;
    val += dir;
    if (val == 255 || val == 0) dir = -dir;   // разворачиваем
    led.setBrightness(val);
  }
}

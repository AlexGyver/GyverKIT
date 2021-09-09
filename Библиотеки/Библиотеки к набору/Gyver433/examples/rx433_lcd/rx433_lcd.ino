// выводим данные на дисплей. Отправляет пример demo_tx
//#define G433_SPEED 1000   // скорость 100-10000 бит/с, по умолч. 1500 бит/с

#include <Gyver433.h>
Gyver433_RX<2> rx;  // указали пин

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3f, 16, 2); // или 0x27

void setup() {
  lcd.init();
  lcd.backlight();
}

void loop() {
  if (rx.tick()) {
    lcd.clear();
    lcd.home();
    for (byte i = 0; i < rx.size; i++) lcd.write(rx.buffer[i]);
  }
}
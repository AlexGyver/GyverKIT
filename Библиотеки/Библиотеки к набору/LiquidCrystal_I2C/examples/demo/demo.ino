#include <LiquidCrystal_I2C.h>

// адрес дисплея 0x3f или 0x27
// размер дисплея 16x2 (поддерживаются и другие, например 20x4)
LiquidCrystal_I2C lcd(0x3f, 16, 2);

void setup() {
  lcd.init();           // инициализация
  lcd.backlight();      // включить подсветку
  //lcd.noBacklight();  // выключить подсветку

  lcd.clear();          // очистить
  lcd.setCursor(0, 0);  // установить курсор (x, y)
  //lcd.home();         // установить курсор в (0, 0)
  
  lcd.setCursor(1, 0);  // место 1 строка 0
  lcd.print("Hello, world!");
  lcd.setCursor(4, 1);  // место 4 строка 1
  lcd.print("GyverKIT");
}

void loop() {
}

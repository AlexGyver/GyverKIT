// тест работы виртуального фейдера (плавные переходы между цветами)

#include <GRGB.h>
// указать тип драйвера COMMON_CATHODE/COMMON_ANODE
// и пины в порядке R,G,B
GRGB led(COMMON_CATHODE, 9, 10, 11);

void setup() {
  Serial.begin(9600);

  // подключить свою функцию для обработки
  led.attach(func);

  // включить фейдер
  led.fadeMode(true);
  // теперь любая установка цвета будет плавной

  // установить период перехода
  led.setFadePeriod(1000);

  // включить CRT для красоты
  led.setCRT(true);
}

// наша функция, будет вызываться при смене цвета
void func() {
  // тут можно управлять внешним драйвером,
  // получая готовый сигнал из библиотеки
  Serial.print(led.R); Serial.print(',');
  Serial.print(led.G); Serial.print(',');
  Serial.println(led.B);
  // ОТКРОЙ ПЛОТТЕР И СМОТРИ ГРАФИКИ =)
}

void loop() {
  // тикер. Вызывать как можно чаще, он сам меняет цвет
  // выполняется "асинхронно"
  led.tick();

  // имитируем переключение цвета (по таймеру)
  static uint32_t tmr = 0;
  static byte mode = 0;

  // таймер на 2 секунды
  if (millis() - tmr >= 2000) {
    tmr = millis();
    if (++mode >= 5) mode = 0; // mode меняется от 0 до 4
    switch (mode) {
      case 0: led.setRGB(255, 0, 0);      // яркий красный
        break;
      case 1: led.setRGB(255, 0, 0, 50);  // приглушённый красный (ярк 50)
        break;
      case 2: led.setColor(GAqua);        // бирюзовый яркий
        break;
      case 3: led.setColor(GMaroon);      // тёмно красный
        break;
      case 4: led.setWheel8(30, 10);      // ~тёмно зелёный
        break;
    }
  }
}

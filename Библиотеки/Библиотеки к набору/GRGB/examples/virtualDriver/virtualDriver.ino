// виртуальный драйвер
// также будет работать с фейдером, смотри пример virtualFader

#include <GRGB.h>
// указать тип драйвера COMMON_CATHODE/COMMON_ANODE
GRGB led(COMMON_CATHODE);

void setup() {
  Serial.begin(9600);

  // подключить свою функцию для обработки
  led.attach(func);

  led.setRGB(100, 0, 255);
  delay(1000);

  led.setBrightness(50);
  delay(1000);
}

// наша функция, будет вызываться при смене цвета
void func() {
  // тут можно управлять внешним драйвером,
  // получая готовый сигнал из библиотеки
  Serial.print(led.R); Serial.print(',');
  Serial.print(led.G); Serial.print(',');
  Serial.println(led.B);
}

void loop() {
}

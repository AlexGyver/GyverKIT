// чтение, асинхронный вариант с тикером

#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // функция опрашивает датчик по своему таймеру
  if (htu.readTick()) {
    // можно забирать значения здесь
    Serial.println(htu.getTemperature());
    Serial.println(htu.getHumidity());
    Serial.println();
  }
}

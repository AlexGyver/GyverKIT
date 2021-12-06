// чтение, асинхронный вариант

#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // Читаем температуру
  htu.requestTemperature();                 // Запрашиваем преобразование
  delay(100);                               // Ждем окончания (см. доку)
  if (htu.readTemperature()) {              // Читаем температуру из датчика и проверяем
    Serial.print("Temp: ");
    Serial.print(htu.getTemperature());     // В случае успеха выводим температуру в порт
    Serial.println(" *C");
  }

  // Читаем влажность
  htu.requestHumidity();                    // Запрашиваем преобразование
  delay(100);                               // Ждем окончания (см. доку)
  if (htu.readHumidity()) {                 // Читаем влажность из датчика и проверяем
    Serial.print("Hum: ");
    Serial.print(htu.getHumidity());        // В случае успеха выводим влажность в порт
    Serial.println(" %");
  }

  Serial.println();
  delay(1000);
}

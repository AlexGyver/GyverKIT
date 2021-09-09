// передача структуры данных

//#define G433_SPEED 1000   // скорость 100-10000 бит/с, по умолч. 2000 бит/с
#define G433_SLOW // отправляю раз в секунду на SYN480R

#include <Gyver433.h>
Gyver433_TX<2> tx;  // указали пин

// формат пакета для отправки
struct DataPack {
  byte counter = 0;
  byte randomNum;
  int analog;
  uint32_t time;
};
DataPack data;

void setup() {
  Serial.begin(9600);
}

void loop() {
  data.counter++;                 // тут счётчик
  data.randomNum = random(256);   // случайное число
  data.analog = analogRead(0);    // тут ацп
  data.time = millis();           // тут миллис
  tx.sendData(data);

  Serial.println("Transmitted:");
  Serial.println(data.counter);
  Serial.println(data.randomNum);
  Serial.println(data.analog);
  Serial.println(data.time);
  Serial.println();

  delay(1000);
}

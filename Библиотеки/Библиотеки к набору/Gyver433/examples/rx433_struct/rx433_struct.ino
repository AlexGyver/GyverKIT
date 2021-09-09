// приём структуры данных

//#define G433_SPEED 1000   // скорость 100-10000 бит/с, по умолч. 2000 бит/с

#include <Gyver433.h>
Gyver433_RX<2, 12> rx;  // указали пин и размер буфера

// формат пакета для приёма (такой же как отправляется)
struct DataPack {
  byte counter;
  byte randomNum;
  int analog;
  uint32_t time;
};

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (rx.tick()) {
    DataPack data;            // "буферная" структура
    if (rx.readData(data)) {  // переписываем данные в неё
      // если данные подходят - выводим
      Serial.println(data.counter);
      Serial.println(data.randomNum);
      Serial.println(data.analog);
      Serial.println(data.time);
      Serial.println();
    } else {
      Serial.println("Wrong data");
    }
  }
}

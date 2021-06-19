// приём структуры данных

//#define G433_SPEED 1000   // скорость 100-8000 бит/с, по умолч. 2000 бит/с

#include <Gyver433.h>
Gyver433_RX<2, 10> rx;  // указали пин и размер буфера

// формат пакета для приёма (такой же как отправляется)
struct dataPack {
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
    dataPack data;     // "буферная" структура
    rx.readData(data); // переписываем данные в неё

    Serial.println("Received:");
    Serial.println(data.counter);
    Serial.println(data.randomNum);
    Serial.println(data.analog);
    Serial.println(data.time);
    Serial.println();
  }
}

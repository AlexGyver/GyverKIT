// приём в прерывании. Отправляет пример demo_tx
//#define G433_SPEED 1000   // скорость 100-8000 бит/с, по умолч. 2000 бит/с

#include <Gyver433.h>
Gyver433_RX<2, 20> rx;  // указали пин и размер буфера

void setup() {
  Serial.begin(9600);
  // взводим прерывания по CHANGE
  attachInterrupt(0, isr, CHANGE);
}

void isr() {
  rx.tickISR();  // спец. тикер вызывается в прерывании по CHANGE
}

void loop() {
  // .gotData() вернёт количество удачно принятых в прерывании байт  
  if (rx.gotData()) {   // если больше 0
    Serial.write(rx.buffer, rx.size);
    Serial.println();
  }

  // имитация загруженного кода
  delay(100);
}

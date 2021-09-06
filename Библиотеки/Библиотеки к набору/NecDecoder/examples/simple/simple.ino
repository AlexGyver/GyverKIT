// приём команд с пульта
// приёмник подключать на прерывание по FALLING

#include <NecDecoder.h>
NecDecoder ir;

void setup() {
  Serial.begin(9600);
  // подключил на D2, прерывание 0
  attachInterrupt(0, irIsr, FALLING);
}

// в прерывании вызываем tick()
void irIsr() {
  ir.tick();
}

void loop() {
  // если пакет успешно принят
  if (ir.available()) {      
    // выводим весь пакет (32 бита)
    Serial.print("0x");
    Serial.println(ir.readPacket(), HEX);

    // можно вывести только команду (8 бит)
    //Serial.println(ir.readCommand(), HEX);
  }
}
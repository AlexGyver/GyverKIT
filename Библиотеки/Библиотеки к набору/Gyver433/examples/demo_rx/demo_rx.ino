// демо-пример приёма в прерывании
// SYN480R, крупный чип: 3.3-5.5V
// MX-RM-5V (RF-5V): 5V
// RX470 (WL101-341): 3-5V

// дефайны перед подключением библиотеки
//#define G433_SPEED 2000   // скорость 100-10000 бит/с, по умолч. 2000 бит/с

#include <Gyver433.h>
//Gyver433_RX<пин> rx;
//Gyver433_RX<пин, буфер> rx;

// пин: цифровой пин
// буфер: размер приёмного буфера в байтах. По умолч. 64

#include <Gyver433.h>
Gyver433_RX<2, 20> rx;

void setup() {
  Serial.begin(9600);
  // взводим прерывания по CHANGE
  attachInterrupt(0, isr, CHANGE);
}

// тикер вызывается в прерывании
void isr() {
  rx.tickISR();
}

void loop() {  
  // gotData() вернёт количество удачно принятых байт
  if (rx.gotData()) {   // если больше 0    
    // ЧИТАЕМ. СПОСОБ 1
    // я знаю, что передатчик отправляет char[15]
    char data[15];
    
    // читаем принятые данные в data
    // если данные совпадают по размеру - ок
    if (rx.readData(data)) Serial.print(data);
    else Serial.print("Data error");

    // ЧИТАЕМ. СПОСОБ 2
    // вывести сырые данные из буфера в порт
    //Serial.write(rx.buffer, rx.size);
    
    // выведем также качество соединения
    Serial.print(", RSSI: ");
    Serial.println(rx.getRSSI());
  }

  // имитация загруженного кода. Не влияет на приём
  delay(50);
}

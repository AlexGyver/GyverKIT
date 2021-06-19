// обмен сырыми данными без CRC и буфера на отправку
// принимает пример raw_rx

//#define G433_SPEED 1000   // скорость 100-8000 бит/с, по умолч. 2000 бит/с

#include <Gyver433.h>
Gyver433_TX<2, 0, G433_NOCRC> tx; // буфер на отправку 0! Экономим

void setup() {
}

char data[] = "Hello from #xx"; // строка для отправки
byte count = 0;                 // счётчик для отправки

void loop() {
  // добавляем счётчик в строку
  data[12] = (count / 10) + '0';
  data[13] = (count % 10) + '0';
  if (++count >= 100) count = 0;

  // отправка данных типа byte*
  tx.write((byte*)data, sizeof(data));

  // отправка 10 раз в сек
  delay(100);
}

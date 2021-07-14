![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# Gyver433
Библиотека для радиомодулей 433 МГц и Arduino
- Супер лёгкая либа, заведётся даже на тини13 (отправка)
- Поддержка кривых китайских модулей
- Интерфейс Manchester или Pulselength
- Встроенный CRC контроль целостности (CRC8 или XOR)
- Ускоренный алгоритм IO для AVR Arduino
- Опционально работа в прерывании (приём данных)

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **Gyver433** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/Gyver433/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
// === КЛАССЫ ===
// Gyver433_RX - приёмник
// Gyver433_TX - передатчик

// === ИНИЦИАЛИЗАЦИЯ ===
Gyver433_xx<пин> xx;
Gyver433_xx<пин, буфер> xx;
Gyver433_xx<пин, буфер, CRC> xx;
// пин: цифровой пин
// буфер: размер буфера в байтах. На "ручную" отправку буфер не нужен. По умолч. 64
// CRC: проверка целостности данных: G433_CRC8 (надёжный), G433_XOR (лёгкий), G433_NOCRC (отключено). По умолч. G433_CRC8


// === ДЕФАЙНЫ-НАСТРОЙКИ ===
// вызывать перед подключением библиотеки
#define G433_FAST           // [TX] короткая синхронизация для зелёных модулей
#define G433_MEDIUM         // [TX] средняя синхронизация при отправке на SYN480R ЧАЩЕ 400мс (активно по умолчанию)
#define G433_SLOW           // [TX] длинная синхронизация при отправке на SYN480R РЕЖЕ 400мс
#define G433_MANCHESTER     // [должно быть одинаково на RX и TX] интерфейс Manchester Coding для экспериментов =) 
#define G433_SPEED 1000     // [должно быть одинаково на RX и TX] скорость 100-8000 бит/с, по умолч. 2000 бит/с 
#define G433_RSSI_COUNT 10  // [RX] количество успешно принятых пакетов для расчёта RSSI (по умолч. 10)
```

<a id="usage"></a>
## Использование
```cpp
// ========= Gyver433_TX =========
void sendData(T &data);                 // отправить данные любого типа (CRC добавляется автоматически)
void write(uint8_t* buf, uint8_t size); // отправить массив байт указанного размера (CRC не добавляется)
uint8_t buffer[];                       // доступ к буферу для отладки

// ========= Gyver433_RX =========
uint16_t tick();            // неблокирующий приём, вернёт кол-во успешно принятых байт
uint16_t tickWait();        // блокирующий приём (более надёжный), вернёт кол-во успешно принятых байт
uint16_t tickISR();         // тикер для прерывания по CHANGE (см. пример isr_rx)
bool readData(T &data);     // прочитает буфер в любой тип данных (в указанную переменную)
uint16_t getSize();         // получить размер принятых данных
uint16_t gotData();         // вернёт количество успешно принятых в tickISR() байт (см. пример isr_rx)
uint8_t buffer[];           // доступ к буферу для отладки
uint8_t getRSSI();          // получить качество приёма (процент успешных передач)

// ============= CRC =============
// можно использовать встроенные функции для генерации байта CRC для ручной упаковки пакетов
uint8_t G433_crc8(uint8_t *buffer, uint8_t size);       // ручной CRC8
uint8_t G433_crc_xor(uint8_t *buffer, uint8_t size);    // ручной CRC XOR
```

<a id="example"></a>
## Примеры
Остальные примеры смотри в **examples**!
![scheme](/doc/scheme.jpg)
### Отправка
```cpp
#include <Gyver433.h>
Gyver433_TX<2, 20> tx;  // указали пин и размер буфера

void setup() {
}

char data[] = "Hello from #xx"; // строка для отправки
byte count = 0;                 // счётчик для отправки

void loop() {
  // добавляем счётчик в строку
  data[12] = (count / 10) + '0';
  data[13] = (count % 10) + '0';
  if (++count >= 100) count = 0;
  tx.sendData(data);
  delay(100);
}
```

### Приём
```cpp
#include <Gyver433.h>
Gyver433_RX<2, 20> rx;  // указали пин и размер буфера

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (rx.tickWait()) {                  // если успешно принято больше 0
    Serial.write(rx.buffer, rx.size);   // выводим
    Serial.println();
  }
}
```

### Приём в прерывании
```cpp
#include <Gyver433.h>
Gyver433_RX<2, 20> rx;  // указали пин и размер буфера

void setup() {
  Serial.begin(9600);  
  attachInterrupt(0, isr, CHANGE);  // прерывание пина радио по CHANGE
}

void isr() {
  rx.tickISR();     // спец. тикер вызывается в прерывании
}

void loop() {
  if (rx.gotData()) {                   // если успешно принято больше 0
    Serial.write(rx.buffer, rx.size);   // выводим
    Serial.println();
  }  
  delay(200);                           // имитация загруженного кода
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - оптимизация, новый интерфейс, поддержка дешёвых синих модулей, работа в прерывании
- v1.2 - улучшение качества связи, оптимизация работы в прерывании
- v1.3 - добавлен вывод RSSI

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
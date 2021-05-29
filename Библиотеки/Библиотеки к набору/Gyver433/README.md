![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# Gyver433
Библиотека для радиомодулей 433 МГц и Arduino
- Не использует прерывания и таймеры (кроме нулевого, читает micros())
- Встроенный CRC контроль целостности
- Ускоренный алгоритм IO для AVR Arduino

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
// указать пин
Gyver433_RX rx(2);
Gyver433_TX tx(2);
```

<a id="usage"></a>
## Использование
```cpp
// ========= Gyver433_TX =========
void sendData(T &data);     // отправить данные любого типа

// ========= Gyver433_RX =========
uint8_t tick();             // неблокирующий приём, вернёт кол-во успешно принятых байт
uint8_t tickWait();         // блокирующий приём, вернёт кол-во успешно принятых байт
bool readData(T &data);     // прочитает буфер в любой тип данных (в указанную переменную)
int getSize();              // получить размер принятых данных
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!  
### Отправка
```cpp
// мелкий передатчик 3.6V SYN115

#define G433_BUFSIZE 50   // размер буфера
#define G433_SPEED 2000   // скорость бит/сек (минимальная)

#include <Gyver433.h>
Gyver433_TX tx(2);  // указали пин

void setup() {
}

char data[] = "Hello from #xx";
byte count = 0;
void loop() {
  data[12] = (count / 10) + '0';
  data[13] = (count % 10) + '0';
  if (++count >= 100) count = 0;
  tx.sendData(data);  
  delay(100);
}
```

### Приём
```cpp
// крупный приёмник 5.0 SYN480R

#define G433_BUFSIZE 50   // размер буфера
#define G433_SPEED 2000   // скорость бит/сек (минимальная)

#include <Gyver433.h>
Gyver433_RX rx(2);

void setup() {
  Serial.begin(9600);
}


void loop() {
  // tick принимает асинхронно, но может ловить ошибки при загруженном коде
  // tickWait блокирует выполнение, но принимает данные чётко
  if (rx.tickWait()) {
    byte buf[64];
    rx.readData(buf);
    for (byte i = 0; i < rx.size; i++) Serial.write(buf[i]);
  }
}
```

<a id="versions"></a>
## Версии
- v1.0

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
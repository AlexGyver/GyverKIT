![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# NecDecoder
Лёгкая библиотека для декодирования ИК протокола NEC (китайские пульты)
- Библиотека не забирает никакие прерывания
- Отсчет времени на micros()
- Обработка команды повтора (удержание кнопки пульта)
- Обработка и чтение занимает 500б Flash

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
- Библиотеку можно найти по названию **NecDecoder** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/NecDecoder/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
// чтение
NecDecoder ir;

// отправка
NecEncoder enc(pin);
NecEncoder enc(pin, del);
// pin - любой цифровой пин
// del - количество мкс, чтобы получить 38 кгц. По умолч 10
// (из расчёта 10мкс задержка + 3 мкс выполнение digitalWrite Arduino)
```

<a id="usage"></a>
## Использование
```cpp
// NecDecoder
void tick();                        // Вызывать при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника в прерывании
bool available();                   // Возвращает true, если корректный пакет прочитан или повторён (isDecoded() + isRepeated())
uint32_t readPacket();              // Прочитать пакет целиком (адрес + ~адрес + команда + ~команда)
uint8_t readAddress();              // Прочитать адрес
uint8_t readCommand();              // Прочитать команду
bool isDecoded();                   // Возвращает true, если пакет успешно декодирован
bool isRepeated();                  // Возвращает true, если принят флаг повтора команды

// NecEncoder
void send(uint8_t address, uint8_t data);   // отправить адрес и данные
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!  
**Приём**
```cpp
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
```

**Отправка**
```cpp
#include <NecEncoder.h>
NecEncoder enc(3);

void setup() {
}

void loop() {
  enc.send(0x15, 0xaa);
  delay(1000);
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - исправлены ошибки, добавлена возможность подключения обработчиков, добавлен контроль потока
- v2.0 - завёз очень много оптимизации, стабильноси, надёжности, упрощения и удобства (by AlexGyver)
- v2.1 - добавил отправку (NecEncoder)

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
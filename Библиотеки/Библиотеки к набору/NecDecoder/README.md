![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# NecDecoder
Лёгкая библиотека для декодирования ИК протокола NEC (Японский вариант)
- Требует вызова специального метода при отрицательном (FALLING) фронте на пине ИК приемника
- Отсчет времени базируется на micros()
- Хранение ОДНОГО последнего декодированного пакета данных в буфере
- Обработка команды повтора (удержания кнопки пульта)

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
NecDecoder ir;
```

<a id="usage"></a>
## Использование
```cpp
void tick();                                // Вызывается при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника, например в прерывании
void attachDecode(void (*handler)(void));   // 'Подключить' обработчик, вызываемый при декодировании очередного пакета (не обязательно)
void attachRepeat(void (*handler)(void));   // 'Подключить' обработчик, вызываемый при получении команды повтора (не обязательно)
bool isDecoded();                           // Возвращает true, когда очередной пакет декодирован
bool isRepeated();                          // Возвращает true, если принят флаг повтора команды
uint32_t readPacket();                      // Прочитать весь пакет целиком (адрес + ~адрес + команда + ~команда)
uint8_t readAddress();                      // Прочитать только байт с адресом
uint8_t readInvAddress();                   // Прочитать только байт с инвертированным адресом
uint8_t readCommand();                      // Прочитать только байт с командой
uint8_t readInvCommand();                   // Прочитать только байт с инвертированной командой
bool addressIsValid();                      // Вернет true, если адрес прошел проверку
bool commandIsValid();                      // Вернет true, если команда прошла проверку
bool packetIsValid();                       // Вернет true, если весь пакет прошел проверку
```
Если в загруженном коде (с кучей прерываний) команды распознаются не всегда - попробуй увеличить значение `_NEC_TOLERANCE` в заголовочном файле библиотеки NecDecoder.h, строка ~32

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!
```cpp
#include "NecDecoder.h"

NecDecoder ir;

void ir_edge() {	// Обработчик внешнего прерывания INT0 по спаду FALLING (ОБЯЗАТЕЛЬНО!)
  ir.tick();  		// Вызываем тикер по спаду
}

void setup() {
  Serial.begin(9600);						// Открываем порт
  attachInterrupt(0, ir_edge, FALLING);		// Внешнее прерывание INT0, по спаду FALLING
}

void loop() {
  if (ir.isDecoded()) {          				 // Если поймали пакет
    if (ir.packetIsValid()) {    				 // И он прошел проверку (еще есть ir.addressIsValid() и ir.commandIsValid())
      Serial.println("\nNEC packet is decoded"); 
      Serial.print("Packet: 0x");
      Serial.println(ir.readPacket(), HEX);      // Пакет целиком
      Serial.print("Address: 0x");
      Serial.println(ir.readAddress(), HEX);     // Только адрес
      Serial.print("~Address: 0x");
      Serial.println(ir.readInvAddress(), HEX);  // Только инвертированный адрес
      Serial.print("Command: 0x");
      Serial.println(ir.readCommand(), HEX);     // Только комманда
      Serial.print("~Command: 0x");
      Serial.println(ir.readInvCommand(), HEX);  // Только инвертированная команда
    } else {
	  Serial.println("\nNEC packet is damaged"); // Иначе - пакет поврежден		
	}
  } else if (ir.isRepeated()) {                  // Если поймали код повтора
    Serial.print("\nRepeat: 0x");                // Выводим последний принятый пакет
    Serial.println(ir.readPacket(), HEX);		
  }
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - исправлены ошибки, добавлена возможность подключения обработчиков, добавлен контроль потока 

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
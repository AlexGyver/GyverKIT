![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# GyverHTU21D
Лёгкая библиотека для датчика температуры и влажности HTU21D
- Асинхронное чтение
- Блокирующее чтение
- Установка разрешения
- Проверка напряжения
- Управление нагревателем

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
- Библиотеку можно найти по названию **GyverHTU21D** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverHTU21D/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
GyverHTU21D htu;
```

<a id="usage"></a>
## Использование
```cpp
bool begin();                       // запуск и проверка наличия датчика
void setHeater(bool state);         // Включение / выключение подогрева
bool powerGood();                   // Состояние питания
void setResolution(uint8_t mode);   // Установка разрешения

// Разрешение:
HTU21D_RES_HIGH     // Высокое разрешение (см. таблицу ниже) - по умолчанию
HTU21D_RES_MEDIUM   // Среднее разрешение (см. таблицу ниже)
HTU21D_RES_LOW      // Низкое разрешение (см. таблицу ниже)

bool readTick(uint16_t prd = 100);  // асинхронный тикер чтения, true если готово. Можно указать свой период. По умолч. 100 мс

bool requestTemperature();          // Запросить температуру (true если успешно)
bool requestHumidity();             // Запросить влажность (true если успешно)

bool readTemperature();             // Прочитать температуру (true если успешно)
bool readHumidity();                // Прочитать влажность (true если успешно)

float getTemperature();             // Получить температуру
float getHumidity();                // Получить влажность

float getTemperatureWait();         // прочитать температуру (блокирующая функция)
float getHumidityWait();            // прочитать флажность (блокирующая функция)
```

### Разрешение и время преобразования
Режим                 |Разрешение, темп.  |Разрешение, влажн. |Время чтения, темп.|Время чтения, влажн.   |
----------------------|-------------------|-------------------|-------------------|-----------------------|
**HTU21D_RES_LOW**    |12 bit             |8 bit              |13 ms              |3 ms                   |
**HTU21D_RES_MEDIUM** |11 bit             |11 bit             |4 ms               |8 ms                   |
**HTU21D_RES_HIGH**   |14 bit             |12 bit             |50 ms              |16 ms                  |

### Блокирующее чтение
Значения можно просто получить из `getTemperatureWait()` и `getHumidityWait()`. Функции выполняются время, указанное в таблице выше.

### Асинхронное чтение (вручную)
Режим асинхронного чтения позволяет не ждать преобразование и вернуться к опросу позже, не блокируя программу. 
В этом режиме нужно запросить чтение, подождать время согласно разрешению, а затем прочитать значение. Запросить 
можно только одну измеряемую величину за раз: запросили температуру - прочитали температуру. Запросили влажность - 
прочитали влажность. Запросить сразу и то и то нельзя!
- Запросить измерение: вызвать `requestTemperature()` или `readTemperature()`. Вернёт `true` при успешном запросе.
- Прочитать измерение: `readTemperature()` или `readHumidity()`. Вернёт `true` при успешном чтении.
- Получить результат: `getTemperature()` или `getHumidity()`.

### Асинхронное чтение (автоматически)
В этом режиме библиотека сама опрашивает и читает датчик по встроенному таймеру, остаётся только забрать результат. 
Просто в цикле вызываем `readTick()` или `readTick(свой период в мс)`. Если функция вернула `true` - данные обновились.

<a id="example"></a>
## Пример
### Блокирующее чтение
```cpp
#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // время выполнения зависит от разрешения, см. доку
  float temp = htu.getTemperatureWait();
  float hum = htu.getHumidityWait();
  
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" *C");
  
  Serial.print("Hum: ");
  Serial.print(hum);
  Serial.println(" %");
  
  Serial.println();
  delay(1000);
}
```

### Ручное асинхронное чтение
```cpp
#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // Читаем температуру
  htu.requestTemperature();                 // Запрашиваем преобразование
  delay(100);                               // Ждем окончания (см. доку)
  if (htu.readTemperature()) {              // Читаем температуру из датчика и проверяем
    Serial.print("Temp: ");
    Serial.print(htu.getTemperature());     // В случае успеха выводим температуру в порт
    Serial.println(" *C");
  }

  // Читаем влажность
  htu.requestHumidity();                    // Запрашиваем преобразование
  delay(100);                               // Ждем окончания (см. доку)
  if (htu.readHumidity()) {                 // Читаем влажность из датчика и проверяем
    Serial.print("Hum: ");
    Serial.print(htu.getHumidity());        // В случае успеха выводим влажность в порт
    Serial.println(" %");
  }

  Serial.println();
  delay(1000);
}
```

### Автоматическое асинхронное чтение
```cpp
#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // функция опрашивает датчик по своему таймеру
  if (htu.readTick()) {
    // можно забирать значения здесь или в другом месте в программе
    Serial.println(htu.getTemperature());
    Serial.println(htu.getHumidity());
    Serial.println();
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
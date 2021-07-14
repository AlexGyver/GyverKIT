![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# microDS3231
Лёгкая библиотека для работы с RTC DS3231 для Arduino
- Чтение и запись времени
- Вывод в char* и String
- Чтение температуры датчика

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
- Библиотеку можно найти по названию **microDS3231** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/microDS3231/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
MicroDS3231 rtc;        // адрес по умолчанию 0x68
MicroDS3231 rtc(адрес); // указать свой адрес
```

<a id="usage"></a>
## Использование
```cpp
void setTime(uint8_t param);	// установка времени == времени компиляции
void setTime(DateTime time);	// установить из структуры DateTime
void setTime(int8_t seconds, int8_t minutes, int8_t hours, int8_t date, int8_t month, int16_t year);	// установка времени
void setHMSDMY(int8_t hours, int8_t minutes, int8_t seconds, int8_t date, int8_t month, int16_t year);	// установка времени тип 2
    
DateTime getTime(void);			// получить в структуру DateTime
uint8_t getSeconds(void);		// получить секунды
uint8_t getMinutes(void);		// получить минуты
uint8_t getHours(void);			// получить часы
uint8_t getDay(void);			// получить день недели
uint8_t getDate(void);			// получить число
uint16_t getYear(void);			// получить год
uint8_t getMonth(void);			// получить месяц
    
String getTimeString();			// получить время как строку вида HH:MM:SS
String getDateString();			// получить дату как строку вида DD.MM.YYYY
void getTimeChar(char* array);	// получить время как char array [8] вида HH:MM:SS
void getDateChar(char* array);	// получить дату как char array [10] вида DD.MM.YYYY       
    
bool lostPower(void);			// проверка на сброс питания
float getTemperatureFloat(void);// получить температуру float
int getTemperature(void);		// получить температуру int
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!
```cpp
// демо возможностей библиотеки
#include <microDS3231.h>
MicroDS3231 rtc;

void setup() {
  Serial.begin(9600);
  
  if (rtc.lostPower()) {  //  при потере питания
    rtc.setTime(COMPILE_TIME);  // установить время компиляции
  }

  //rtc.setTime(SEC, MIN, HOUR, DAY, MONTH, YEAR); // устанвока времени вручную
}

void loop() {
  printTime();
  delay(500);
}

void printTime() {
  Serial.print(rtc.getHours());
  Serial.print(":");
  Serial.print(rtc.getMinutes());
  Serial.print(":");
  Serial.print(rtc.getSeconds());
  Serial.print(" ");
  Serial.print(rtc.getDay());
  Serial.print(" ");
  Serial.print(rtc.getDate());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.println(rtc.getYear());
  
  Serial.println(rtc.getTemperatureFloat());  
  //Serial.println(rtc.getTemperature());
  Serial.println(rtc.getTimeString());
  Serial.println(rtc.getDateString());

  // работа с char
  char time[8];
  rtc.getTimeChar(time);  // записать в массив time
  Serial.println(time);
  
  char date[10];
  rtc.getDateChar(date);
  Serial.println(date);
  
  Serial.println();
}
```

<a id="versions"></a>
## Версии
- v1.2 - добавлены ограничения на вводимые в setTime числа. Также нельзя ввести 29 февраля увы =)
- v1.3 - пофикшено зависание, когда модуль отключен но опрашивается
- v1.4 - незначительный фикс
- v2.0 - новые возможности, оптимизация и облегчение
- v2.1 - добавил вывод температуры, вывод в String и char
- v2.2 - исправлены дни недели (пн-вс 1-7)
- v2.3 - небольшие исправления, оптимизация, изменён порядок вывода даты
- v2.4 - исправлена установка времени компиляции
    
<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
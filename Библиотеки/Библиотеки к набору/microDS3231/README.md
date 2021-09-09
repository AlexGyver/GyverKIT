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
bool begin();                   // инициализация, вернет true, если RTC найден
void setTime(uint8_t param);	// установка времени == времени компиляции
void setTime(DateTime time);	// установить из структуры DateTime
void setTime(int8_t seconds, int8_t minutes, int8_t hours, int8_t date, int8_t month, int16_t year);	// установка времени
void setHMSDMY(int8_t hours, int8_t minutes, int8_t seconds, int8_t date, int8_t month, int16_t year);	// установка времени тип 2
    
// структура DateTime
uint8_t second; 
uint8_t minute;
uint8_t hour;
uint8_t day;
uint8_t date;
uint8_t month;
uint16_t year;

DateTime getTime();			// получить в структуру DateTime
uint8_t getSeconds();		// получить секунды
uint8_t getMinutes();		// получить минуты
uint8_t getHours();			// получить часы
uint8_t getDay();			// получить день недели
uint8_t getDate();			// получить число
uint16_t getYear();			// получить год
uint8_t getMonth();			// получить месяц
    
String getTimeString();			// получить время как строку вида HH:MM:SS
String getDateString();			// получить дату как строку вида DD.MM.YYYY
void getTimeChar(char* array);	// получить время как char array [8] вида HH:MM:SS
void getDateChar(char* array);	// получить дату как char array [10] вида DD.MM.YYYY       
    
bool lostPower();			// проверка на сброс питания
float getTemperatureFloat();// получить температуру float
int getTemperature();		// получить температуру int
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
  
  // проверка наличия модуля на линии i2c
  // вызов rtc.begin() не обязателен для работы
  if (!rtc.begin()) {
    Serial.println("DS3231 not found");
    for(;;);
  }
  
  // ======== УСТАНОВКА ВРЕМЕНИ АВТОМАТИЧЕСКИ ========
  // rtc.setTime(COMPILE_TIME);     // установить время == времени компиляции
  
  // визуально громоздкий, но более "лёгкий" с точки зрения памяти способ установить время компиляции
  rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
    
  if (rtc.lostPower()) {            // выполнится при сбросе батарейки
    Serial.println("lost power!");
    // тут можно однократно установить время == времени компиляции
  }
  
  // ======== УСТАНОВКА ВРЕМЕНИ ВРУЧНУЮ ========    
  // установить время вручную можно двумя способами (подставить реальные числа)
  //rtc.setTime(SEC, MIN, HOUR, DAY, MONTH, YEAR);
  //rtc.setHMSDMY(HOUR, MIN, SEC, DAY, MONTH, YEAR);
  
  // также можно установить время через DateTime
  /*
  DateTime now;
  now.second = 0;
  now.minute = 10;
  now.hour = 50;
  now.date = 2;
  now.month = 9;
  now.year = 2021;
  
  rtc.setTime(now);  // загружаем в RTC
  */
}

void loop() {
  // получение и вывод каждой компоненты
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
  
  /*
  // можно через DateTime (более оптимально):
  DateTime now = rtc.getTime();
  Serial.print(now.hour);
  Serial.print(":");
  Serial.print(now.minute);
  Serial.print(":");
  Serial.print(now.second);
  Serial.print(" ");
  Serial.print(now.day);
  Serial.print(" ");
  Serial.print(now.date);
  Serial.print("/");
  Serial.print(now.month);
  Serial.print("/");
  Serial.println(now.year);
  */
  
  // вывод температуры чипа
  Serial.println(rtc.getTemperatureFloat());  
  //Serial.println(rtc.getTemperature());
  
  // вывод времени готовой строкой String
  Serial.println(rtc.getTimeString());
  
  // вывод даты готовой строкой String
  Serial.println(rtc.getDateString());

  // вывод времени через char array
  char time[8];
  rtc.getTimeChar(time);
  Serial.println(time);
  
  // вывод даты через char array
  char date[10];
  rtc.getDateChar(date);
  Serial.println(date);
  
  Serial.println();
  delay(500);
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
- v2.5 - добавлен begin для проверки наличия модуля на линии
    
<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
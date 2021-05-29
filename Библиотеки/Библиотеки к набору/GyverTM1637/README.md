![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# GyverTM1637
GyverTM1637 - бибилотека для 7 сегментного дисплея на чипе TM1637 с кучей приколюх
- Вывод цифр массивом или прицельно
- Вывод букв из списка доступных (листай ниже) массивом или прицельно
- Отдельная функция вывода часов и минут (часы без нуля слева, минуты с нулём)
- Вывод числа от -999 до 9999 с учётом знака
- Готовая функция бегущей строки
- Функции смены яркости и состояния двоеточия автоматически обновляют дисплей
- Функция обновления значения с эффектом вертикальной прокрутки
- Функция обновления значения с эффектом скручивания (лучше один раз увидеть)

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

### Документация
К библиотеке есть [расширенная документация](https://alexgyver.ru/tm1637_display/)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **GyverTM1637** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverTM1637/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
GyverTM1637 disp(CLK, DIO);
```

<a id="usage"></a>
## Использование
```cpp
void display(uint8_t DispData[]);                                       // выводит цифры массивом по ячейкам. От 0 до 9 (byte values[] = {3, 5, 9, 0}; )
void display(uint8_t BitAddr, uint8_t DispData);                        // выводит цифру DispData в указанную ячейку дисплея BitAddr
void display(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);	// если лень создавать массив, выводит цифры в ячейки

void displayByte(uint8_t DispData[]);									// выводит байт вида 0xe6 и буквы-константы вида _a , _b .... массивом
void displayByte(uint8_t BitAddr, uint8_t DispData);                    // выводит байт вида 0xe6 и буквы-константы вида _a , _b .... в ячейку
void displayByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3);	// если лень создавать массив, выводит байты в ячейки

void displayClock(uint8_t hrs, uint8_t mins);							// выводит часы и минуты
void displayClockScroll(uint8_t hrs, uint8_t mins, int delayms);		// выводит часы и минуты с эффектом прокрутки
void displayClockTwist(uint8_t hrs, uint8_t mins, int delayms);			// выводит часы и минуты с эффектом скрутки

void displayInt(int value);												// выводит число от -999 до 9999 (да, со знаком минус)
void runningString(uint8_t DispData[], byte amount, int delayMs);  		// бегущая строка (array, sizeof(array), задержка в мс)
void clear(void);														// очистить дисплей
void point(boolean PointFlag);											// вкл / выкл точку (POINT_ON / POINT_OFF)
void brightness(uint8_t bright, uint8_t = 0x40, uint8_t = 0xc0);		// яркость 0 - 7	

void scroll(uint8_t BitAddr, uint8_t DispData, int delayms);            // обновить значение прокруткой (адрес, ЦИФРА, задержка в мс)
void scroll(uint8_t DispData[], int delayms);							// обновить значение прокруткой (массив ЦИФР, задержка в мс)
void scroll(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);	// прокрутка посимвольно
void scrollByte(uint8_t BitAddr, uint8_t DispData, int delayms);        // обновить значение прокруткой (адрес, БАЙТ, задержка в мс)
void scrollByte(uint8_t DispData[], int delayms);                       // обновить значение прокруткой (массив БАЙТ, задержка в мс)
void scrollByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);	// прокрутка посимвольно

void twist(uint8_t BitAddr, uint8_t DispData, int delayms);				// обновить значение скручиванием (адрес, ЦИФРА, задержка в мс)
void twist(uint8_t DispData[], int delayms);                            // обновить значение скручиванием (массив ЦИФР, задержка в мс)
void twist(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);	// скрутка посимвольно
void twistByte(uint8_t BitAddr, uint8_t DispData, int delayms);			// обновить значение скручиванием (адрес, БАЙТ, задержка в мс)
void twistByte(uint8_t DispData[], int delayms);                        // обновить значение скручиванием (массив БАЙТ, задержка в мс)
void twistByte(uint8_t bit0, uint8_t bit1, uint8_t bit2, uint8_t bit3, int delayms);    // скрутка посимвольно
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!
```cpp
/*
   Пример вывода на дисплей с регистром TM1637
   показывает все возможности библиотеки GyverTM1637
   AlexGyver Technologies http://alexgyver.ru/
*/

#define CLK 2
#define DIO 3

#include "GyverTM1637.h"
GyverTM1637 disp(CLK, DIO);

uint32_t Now, clocktimer;
boolean flag;

void setup() {
  Serial.begin(9600);
  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)

}

void loop() {
  runningText();
  scrolls();
  scrollClock();
  twists();
  twistClock();
  ints();
  bytes();
  fadeBlink();
  normClock();
}

void twists() {
  // скручивание массив ЦИФР
  byte digs[4] = {3, 5, 7, 1};
  disp.twist(digs, 50);     // скорость прокрутки 100
  delay(1000);

  // скручивание прицельно (ячейка, БАЙТ, скорость)
  disp.twistByte(0, _1, 50);
  delay(1000);

  // скручивание прицельно (ячейка, ЦИФРА, скорость)
  disp.twist(0, 8, 70);
  delay(1000);

  disp.clear();
  delay(200);
  for (byte i = 0; i < 10; i++) {
    disp.twist(3, i, 20);
    delay(200);
  }

  // скручивание массива БАЙТ
  byte troll[4] = {_t, _r, _o, _l};
  disp.twistByte(troll, 50);
  delay(1000);

  // прицельное скручивание БАЙТА (ячейка, байт, скорость)
  disp.twistByte(2, _G, 50);
  delay(1000);
}

void twistClock() {
  byte hrs = 21, mins = 55;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 10000) {   // каждые 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      if (flag) {
        // ***** часы! ****
        mins ++;
        if (mins > 59) {
          mins = 0;
          hrs++;
          if (hrs > 24) hrs = 0;
        }
        // ***** часы! ****
        disp.displayClockTwist(hrs, mins, 35);    // выводим время
      }
    }
  }
  disp.point(0);   // выкл точки
}

void scrolls() {
  // прокрутка массив ЦИФР
  byte digs[4] = {3, 5, 7, 1};
  disp.scroll(digs, 100);     // скорость прокрутки 100
  delay(1000);

  // прокрутка прицельно (ячейка, ЦИФРА, скорость)
  disp.scroll(0, 8, 200);
  delay(1000);

  disp.clear();
  delay(1000);
  for (byte i = 0; i < 10; i++) {
    disp.scroll(3, i, 50);
    delay(400);
  }

  // прокрутка массива БАЙТ
  byte troll[4] = {_t, _r, _o, _l};
  disp.scrollByte(troll, 100);
  delay(1000);

  // прицельная прокрутка БАЙТА (ячейка, байт, скорость)
  disp.scrollByte(2, _G, 50);
  delay(1000);
}

void bytes() {
  // выводим байты из массива
  byte troll[4] = {_t, _r, _o, _l};
  disp.displayByte(troll);
  delay(1000);

  // выводим байты напрямую (4 в скобках)
  disp.displayByte(_L, _O, _L, _empty);
  delay(1000);

  // выводим байты "прицельно"
  disp.displayByte(3, _O);    // 3 ячейка, буква О
  delay(1000);

  // выводим цифры из массива
  byte hell[4] = {6, 6, 6, 6};
  disp.display(hell);
  delay(1000);

  // выводим цифры напрямую (4 в скобках)
  disp.display(1, 2, 3, 4);
  delay(1000);

  // выводим цифры "прицельно"
  disp.display(0, 9);    // 0 ячейка, цифра 9
  delay(1000);
}

void fadeBlink() {
  // пишем HELL
  disp.displayByte(_H, _E, _L, _L);

  Now = millis();
  while (millis () - Now < 3000) {    // 3 секунды
    for (int i = 7; i > 0; i--) {
      disp.brightness(i);   // меняем яркость
      delay(40);
    }
    for (int i = 0; i < 8; i++) {
      disp.brightness(i);   // меняем яркость
      delay(40);
    }
  }
}

void scrollClock() {
  byte hrs = 15, mins = 0;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 10000) {   // каждые 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      if (flag) {
        // ***** часы! ****
        mins ++;
        if (mins > 59) {
          mins = 0;
          hrs++;
          if (hrs > 24) hrs = 0;
        }
        // ***** часы! ****
        disp.displayClockScroll(hrs, mins, 70);    // выводим время
      }
    }
  }
  disp.point(0);   // выкл точки
}

void normClock() {
  byte hrs = 15, mins = 0;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 10000) {   // каждые 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      // ***** часы! ****
      mins ++;
      if (mins > 59) {
        mins = 0;
        hrs++;
        if (hrs > 24) hrs = 0;
      }
      // ***** часы! ****
      disp.displayClock(hrs, mins);   // выводим время функцией часов
    }
  }
  disp.point(0);   // выкл точки
}

void ints() {
  // тупо отправляем цифры
  disp.displayInt(-999);
  delay(500);
  disp.displayInt(-99);
  delay(500);
  disp.displayInt(-9);
  delay(500);
  disp.displayInt(0);
  delay(500);
  disp.displayInt(6);
  delay(500);
  disp.displayInt(66);
  delay(500);
  disp.displayInt(666);
  delay(500);
  disp.displayInt(6666);
  delay(500);
}

void runningText() {
  byte welcome_banner[] = {_H, _E, _L, _L, _O, _empty, _empty,
                           _e, _n, _j, _o, _y, _empty, _empty,
                           _1, _6, _3, _7, _empty, _d, _i, _S, _P, _l, _a, _y
                          };
  disp.runningString(welcome_banner, sizeof(welcome_banner), 200);  // 200 это время в миллисекундах!
}

```

<a id="versions"></a>
## Версии
- v1.4 - поправлены типы данных и ошибки, добавлена совместимость с ESP

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
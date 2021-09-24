![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# GyverMAX7219
Cамая резкая библиотека для матриц MAX7219 на диком западе
- Наследует графику из GyverGFX
- Подключение матриц зигзагом
- Аппаратный и программный SPI
- Невероятная оптимизация

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
- Для работы нужна [GyverGFX](https://github.com/GyverLibs/GyverGFX)
- Библиотеку можно найти по названию **GyverMAX7219** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverMAX7219/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
MAX7219 < W, H, CS > mtrx;            // подключение к аппаратному SPI
// пример: UNO / Nano (CLK - D13, DI - D11, CS - любой пин)

MAX7219 < W, H, CS, DATA, CLK > mtrx; // подключение к любым пинам
// W и H - количество МАТРИЦ по горизонтали и вертикали
// CS, DATA, CLK - номера пинов
```

<a id="usage"></a>
## Использование
```cpp
void begin();                   // запустить
void setRotation(uint8_t rot);  // поворот матриц (0, 1, 2, 3 на 90 град по часовой стрелке)
void setBright(byte value);     // установить яркость [0-15]
void setPower(bool value);      // переключить питание
void clear();                   // очистить
void fill();                    // залить
void fillByte(byte data);       // залить байтом
void dot(int x, int y, byte fill = 1);  // установить точку
bool get(int x, int y);         // получить точку
void update();                  // обновить

// а также наследует всё из GyverGFX:
void dot(int x, int y, uint8_t fill = 1);                           // точка, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void fastLineH(int y, int x0, int x1, uint8_t fill = 1);            // вертикальная линия, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void fastLineV(int x, int y0, int y1, uint8_t fill = 1);            // горизонтальная линия, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void line(int x0, int y0, int x1, int y1, uint8_t fill = 1);        // линия, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void rect(int x0, int y0, int x1, int y1, uint8_t fill = 1);        // прямоугольник, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void roundRect(int x0, int y0, int x1, int y1, uint8_t fill = 1);   // скруглённый прямоугольник, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void circle(int x, int y, int radius, uint8_t fill = 1);            // окружность, fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void bezier(uint8_t* arr, uint8_t size, uint8_t dense, uint8_t fill = 1);   // кривая Безье
void bezier16(int* arr, uint8_t size, uint8_t dense, uint8_t fill = 1);     // кривая Безье 16 бит. fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
void drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert = 0, byte mode = 0);  // битмап
void setCursor(int x, int y);           // установить курсор
void setScale(uint8_t scale);           // масштаб текста
void invertText(bool inv);              // инвертировать текст
void autoPrintln(bool mode);            // автоматический перенос строки
void textDisplayMode(bool mode);        // режим вывода текста GFX_ADD/GFX_REPLACE

// и из Print.h
// print/println любой тип данных
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!
```cpp
#include <GyverMAX7219.h>
MAX7219 < 1, 1, 5 > mtrx;   // одна матрица (1х1), пин CS на D5

void setup() {
  mtrx.begin();       // запускаем
  mtrx.setBright(5);  // яркость 0..15
  //mtrx.rotate(1);   // можно повернуть 0..3, по 90 град по часовой стрелке

  mtrx.dot(0, 0);     // пиксель на координатах 0,0
  mtrx.update();      // показать
  delay(1000);
  mtrx.clear();

  // линии крест накрест
  mtrx.line(0, 0, 7, 7);  // (x0, y0, x1, y1)
  mtrx.line(7, 0, 0, 7);
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // круг
  mtrx.circle(3, 3, 3, GFX_FILL); // х, у, радиус, заливка
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // окружность
  mtrx.circle(3, 3, 3, GFX_STROKE);
  mtrx.update();
  delay(1000);
  mtrx.clear();

  // остальную геометрию смотри в документации
}

void loop() {
}
```

<a id="versions"></a>
## Версии
- v1.0 - релиз
- v1.1 - оптимизирован SPI
- v1.2 - переделан FastIO
- v1.2.1 - исправлен баг в SPI (с 1.2)
- v1.2.2 - убран FastIO
- v1.3 - мелкие доработки и оптимизация, добавил поворот матриц

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
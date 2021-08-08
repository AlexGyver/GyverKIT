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
// подключение к аппаратному SPI (Нано CLK 13, DAT 11)
// указать количество МАТРИЦ по горизонтали и вертикали и CS пин
MAX7219 < 4, 1, 5 > mtrx; // W, H, CS

// подключение к любым пинам
//MAX7219 < 4, 1, 5, 11, 13 > mtrx; // W, H, CS, DATA, CLK
```

<a id="usage"></a>
## Использование
```cpp
void begin();                   // запустить
void setBright(byte value);     // установить яркость [0-15]
void setPower(bool value);      // переключить питание
void clear();                   // очистить
void fill();                    // залить
void fillByte(byte data);       // залить байтом
void dot(int x, int y, byte fill = 1);  // установить точку
bool get(int x, int y);         // получить точку
void update();                  // обновить

// а также наследует всё из GyverGFX:
void dot(int x, int y, uint8_t fill = 1);                           // точка
void fastLineH(int y, int x0, int x1, uint8_t fill = 1);            // вертикальная линия
void fastLineV(int x, int y0, int y1, uint8_t fill = 1);            // горизонтальная линия
void line(int x0, int y0, int x1, int y1, uint8_t fill = 1);        // линия
void rect(int x0, int y0, int x1, int y1, uint8_t fill = 1);        // прямоугольник
void roundRect(int x0, int y0, int x1, int y1, uint8_t fill = 1);   // скруглённый прямоугольник
void circle(int x, int y, int radius, uint8_t fill = 1);            // окружность
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
//#define MAX_SPI_SPEED 500000	// дефайн для изменения скорости SPI, по умолч 1000000
#include <GyverMAX7219.h>

#define AM_W 4*8  // 4 матрицы (32 точки)
#define AM_H 2*8  // 2 матрицы (16 точек)

// подключение к аппаратному SPI (Нано CLK 13, DAT 11)
// указать количество МАТРИЦ по горизонтали и вертикали и CS пин
MAX7219 < AM_W / 8, AM_H / 8, 5 > mtrx; // W, H, CS

// подключение к любым пинам
//MAX7219 < AM_W / 8, AM_H / 8, 5, 11, 13 > mtrx; // W, H, CS, DATA, CLK

void setup() {
}

void loop() {
  // анимации на выбор
  //lines();
  //ball();
  //bezier();
  //bezier2();
  //bigBall();
  net();

  // переинициализация для больших матриц
  static uint32_t tmr;
  if (millis() - tmr >= 2000) {
    tmr = millis();
    mtrx.begin();
  }
}

void net() {
  const byte radius = 2;
  const byte amount = 5;
  static bool start = false;
  static int x[amount], y[amount];
  static int velX[amount], velY[amount];
  if (!start) {
    start = 1;
    for (byte i = 0; i < amount; i++) {
      x[i] = random(10, (AM_W - 1) * 10);
      y[i] = random(10, (AM_H - 1) * 10);
      velX[i] = random(2, 9);
      velY[i] = random(2, 9);
    }
  }
  mtrx.clear();
  for (byte i = 0; i < amount; i++) {
    x[i] += velX[i];
    y[i] += velY[i];
    if (x[i] >= (AM_W - 1 - radius) * 10 || x[i] < radius * 10) velX[i] = -velX[i];
    if (y[i] >= (AM_H - 1 - radius) * 10 || y[i] < radius * 10) velY[i] = -velY[i];
    mtrx.circle(x[i] / 10, y[i] / 10, radius);
  }

  for (int i = 0; i < amount; i++) {
    for (int j = 0; j < amount; j++) {
      if (i != j && dist(x[i] / 10, y[i] / 10, x[j] / 10, y[j] / 10) < 35) mtrx.line(x[i] / 10, y[i] / 10, x[j] / 10, y[j] / 10);
    }
  }
  mtrx.update();
  delay(10);
}
int dist(int x1, int y1, int x2, int y2) {
  int lx = (x2 - x1);
  int ly = (y2 - y1);
  return (sqrt(lx * lx + ly * ly));
}

void bezier2() {
  const byte amount = 3;
  static bool start = false;
  static int x[amount], y[amount];
  static int velX[amount], velY[amount];
  if (!start) {
    start = 1;
    for (byte i = 0; i < amount; i++) {
      x[i] = random(10, (AM_W - 1) * 10);
      y[i] = random(10, (AM_H - 1) * 10);
      velX[i] = random(2, 9);
      velY[i] = random(2, 9);
    }
  }
  mtrx.clear();
  byte bez[(amount + 1) * 2];
  for (byte i = 0; i < amount; i++) {
    x[i] += velX[i];
    y[i] += velY[i];
    if (x[i] >= (AM_W - 1) * 10 || x[i] < 0) velX[i] = -velX[i];
    if (y[i] >= (AM_H - 1) * 10 || y[i] < 0) velY[i] = -velY[i];
    mtrx.dot(x[i] / 10, y[i] / 10, 1);
    bez[i * 2] = x[i] / 10;
    bez[i * 2 + 1] = y[i] / 10;
  }
  bez[amount * 2] = bez[0];
  bez[amount * 2 + 1] = bez[1];

  mtrx.bezier(bez, amount + 1, 8);
  mtrx.update();
  delay(30);
}

void bigBall() {
  mtrx.clear();
  byte radius = 3;
  static int x = (AM_W / 2) * 10, y = (AM_H / 2) * 10;
  static int velX = 17, velY = 9;
  static bool fillFlag = 0;
  x += velX;
  y += velY;
  if (x >= (AM_W - 4) * 10 || x < radius * 10) {
    velX = -velX;
    fillFlag = !fillFlag;
  }
  if (y >= (AM_H - 4) * 10 || y < radius * 10) {
    velY = -velY;
    fillFlag = !fillFlag;
  }

  mtrx.circle(x / 10, y / 10, radius, fillFlag ? GFX_STROKE : GFX_FILL);
  mtrx.update();
  delay(20);
}

void bezier() {
  byte data[] = {0, 0, AM_W / 2, AM_H / 2, 0, AM_H - 1};
  for (int i = 0; i < AM_W; i++) {
    mtrx.clear();
    data[0] = data[4] = AM_W - i;
    data[2] = i;
    mtrx.bezier(data, 3, 6);
    mtrx.update();
    delay(30);
  }
  for (int i = AM_W; i > 0; i--) {
    mtrx.clear();
    data[0] = data[4] = AM_W - i;
    data[2] = i;
    mtrx.bezier(data, 3, 6);
    mtrx.update();
    delay(30);
  }
}

void lines() {
  mtrx.clear();
  for (byte i = 0; i < AM_W - 1; i += 3) {
    mtrx.line(0, 0, i, AM_H);
    mtrx.update();
    delay(30);
  }
  for (int i = AM_H - 1; i >= 0 ; i -= 3) {
    mtrx.line(0, 0, AM_W, i);
    mtrx.update();
    delay(30);
  }
  delay(100);

  mtrx.clear();
  for (int i = AM_W - 1; i > 0; i -= 3) {
    mtrx.line(AM_W - 1, 0, i, AM_H);
    mtrx.update();
    delay(30);
  }
  for (int i = 0; i < AM_H; i += 3) {
    mtrx.line(AM_W - 1, AM_H - 1, 0, i);
    mtrx.update();
    delay(30);
  }
  delay(100);
}

void ball() {
  mtrx.clear();
  static int x, y;
  static int velX = 17, velY = 9;
  x += velX;
  y += velY;
  if (x >= (AM_W - 1) * 10 || x < 0) velX = -velX;
  if (y >= (AM_H - 1) * 10 || y < 0) velY = -velY;

  mtrx.dot(x / 10, y / 10, 1);
  mtrx.dot(x / 10 + 1, y / 10 + 1, 1);
  mtrx.dot(x / 10 + 1, y / 10, 1);
  mtrx.dot(x / 10, y / 10 + 1, 1);
  mtrx.update();
  delay(20);
}
```

<a id="versions"></a>
## Версии
- v1.0 - релиз
- v1.1 - оптимизирован SPI
- v1.2 - переделан FastIO
- v1.2.1 - исправлен баг в SPI (с 1.2)

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
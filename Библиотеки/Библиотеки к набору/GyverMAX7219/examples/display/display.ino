// подключаем дисплей 32х16 (две сборки по 4 матрицы)
// демо 6 эффектов, меняются каждые 5 секунд

//#define MAX_SPI_SPEED 500000	// дефайн для изменения скорости SPI, по умолч 1000000
#include <GyverMAX7219.h>

#define AM_W 32  // 4 матрицы (32 точки)
#define AM_H 16  // 2 матрицы (16 точек)

// дисплей 4х2, пин CS 5, остальные на аппаратный SPI
MAX7219 < 4, 2, 5 > mtrx;

void setup() {
  mtrx.begin();
  mtrx.println("Mtrx");
  mtrx.print("Demo");
  mtrx.update();
  delay(3000);
}

void loop() {
  static uint32_t tmrM;
  static byte mode;
  if (millis() - tmrM >= 5000) {  // 5 секунд
    tmrM = millis();
    if (++mode >= 6) mode = 0;    // меняем режим от 0 до 5
    mtrx.clear();
  }

  switch (mode) {
    case 0: lines(); break;
    case 1: ball(); break;
    case 2: bigBall(); break;
    case 3: net(); break;
    case 4: bezier(); break;
    case 5: bitmap(); break;
  }
}

// редактор тут http://jorydotcom.github.io/matrix-emoji/
const uint8_t bmp[] PROGMEM = {
  0b00100100,
  0b00100100,
  0b01111110,
  0b11011011,
  0b11111111,
  0b11111111,
  0b10100101,
  0b00100100,
};

void bitmap() {
  mtrx.clear();
  static int x, y;
  static int velX = 6, velY = 4;
  x += velX;
  y += velY;
  if (x >= (AM_W - 8) * 10 || x < 0) velX = -velX;
  if (y >= (AM_H - 8) * 10 || y < 0) velY = -velY;
  mtrx.drawBitmap(x / 10, y / 10, bmp, 8, 8);
  mtrx.update();
  delay(30);
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
  delay(20);
}

int dist(int x1, int y1, int x2, int y2) {
  int lx = (x2 - x1);
  int ly = (y2 - y1);
  return (sqrt(lx * lx + ly * ly));
}

void bigBall() {
  mtrx.clear();
  byte radius = 3;
  static int x = (AM_W / 2) * 10, y = (AM_H / 2) * 10;
  static int velX = 9, velY = 5;
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
  delay(30);
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
  for (int i = AM_H; i >= 0; i -= 3) {
    mtrx.line(AM_W - 1, 0, 0, i);
    mtrx.update();
    delay(30);
  }
  delay(100);
}

void ball() {  
  static int x, y;
  static int velX = 17, velY = 12;
  x += velX;
  y += velY;
  if (x >= (AM_W - 1) * 10 || x < 0) velX = -velX;
  if (y >= (AM_H - 1) * 10 || y < 0) velY = -velY;

  mtrx.dot(x / 10, y / 10);
  mtrx.update();
  delay(40);
}

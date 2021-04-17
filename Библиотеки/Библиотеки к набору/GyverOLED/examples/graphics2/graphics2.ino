#include <GyverOLED.h>
GyverOLED<SSD1306_128x64/*, OLED_NO_BUFFER*/> oled;
// можно передать адрес: GyverOLED oled(0x3C);

void setup() {
  Serial.begin(9600);
  oled.init();              // инициализация
  Wire.setClock(400000L);
  oled.clear();
  oled.update();
}

void loop() {
  //lines();
  //ball();
  //bezier();
  bezier2();
  //bigBall();
}

void bezier2() {
  const byte amount = 3;
  static bool start = false;
  static int x[amount], y[amount];
  static int velX[amount], velY[amount];
  if (!start) {
    start = 1;
    for (byte i = 0; i < amount; i++) {
      x[i] = random(10, (128 - 1) * 10);
      y[i] = random(10, (64 - 1) * 10);
      velX[i] = random(5, 20);
      velY[i] = random(5, 20);
    }
  }
  oled.clear();
  int bez[(amount + 1) * 2];
  for (byte i = 0; i < amount; i++) {
    x[i] += velX[i];
    y[i] += velY[i];
    if (x[i] >= (128 - 1) * 10 || x[i] < 0) velX[i] = -velX[i];
    if (y[i] >= (64 - 1) * 10 || y[i] < 0) velY[i] = -velY[i];
    oled.dot(x[i] / 10, y[i] / 10, 1);
    bez[i * 2] = x[i] / 10;
    bez[i * 2 + 1] = y[i] / 10;
  }
  bez[amount * 2] = bez[0];
  bez[amount * 2 + 1] = bez[1];

  oled.bezier(bez, amount + 1, 8);
  oled.update();
}

void bigBall() {
  oled.clear();
  byte radius = 5;
  static int x = (128 / 2) * 10, y = (64 / 2) * 10;
  static int velX = 9, velY = 6;
  static bool fillFlag = 0;
  x += velX;
  y += velY;
  if (x >= (128 - 4) * 10 || x < radius * 10) {
    velX = -velX;
    fillFlag = !fillFlag;
  }
  if (y >= (64 - 4) * 10 || y < radius * 10) {
    velY = -velY;
    fillFlag = !fillFlag;
  }

  oled.circle(x / 10, y / 10, radius, fillFlag ? OLED_STROKE : OLED_FILL);
  oled.update();
}

void bezier() {
  int data[] = {0, 0, 128 / 2, 64 / 2, 0, 64 - 1};
  for (int i = 0; i < 128; i++) {
    oled.clear();
    data[0] = data[4] = 128 - i;
    data[2] = i;
    oled.bezier(data, 3, 7);
    oled.update();
  }
  for (int i = 128; i > 0; i--) {
    oled.clear();
    data[0] = data[4] = 128 - i;
    data[2] = i;
    oled.bezier(data, 3, 7);
    oled.update();
  }
}

void lines() {
  oled.clear();
  for (byte i = 0; i < 128 - 1; i += 3) {
    oled.line(0, 0, i, 64);
    oled.update();
    delay(30);
  }
  for (byte i = 0; i < 64 - 1; i += 3) {
    oled.line(0, 0, 128, i);
    oled.update();
    delay(30);
  }
  delay(100);

  oled.clear();
  for (int i = 128 - 1; i > 0; i -= 3) {
    oled.line(128 - 1, 0, i, 64);
    oled.update();
    delay(30);
  }
  for (int i = 0; i < 64; i += 3) {
    oled.line(128 - 1, 64 - 1, 0, i);
    oled.update();
    delay(30);
  }
  delay(100);
}

void ball() {
  oled.clear();
  static int x, y;
  static int velX = 5, velY = 8;
  x += velX;
  y += velY;
  if (x >= (128 - 1) * 10 || x < 0) velX = -velX;
  if (y >= (64 - 1) * 10 || y < 0) velY = -velY;

  oled.dot(x / 10, y / 10, 1);
  oled.dot(x / 10 + 1, y / 10 + 1, 1);
  oled.dot(x / 10 + 1, y / 10, 1);
  oled.dot(x / 10, y / 10 + 1, 1);
  oled.update();
  delay(10);
}

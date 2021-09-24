// дефайн для изменения скорости SPI, по умолч 1000000
//#define MAX_SPI_SPEED 500000

#include <GyverMAX7219.h>
//MAX7219 < W, H, CS > mtrx;    // подключение к аппаратному SPI
// пример: UNO / Nano (CLK - D13, DI - D11, CS - любой пин)

//MAX7219 < W, H, CS, DATA, CLK > mtrx; // подключение к любым пинам
// W и H - количество МАТРИЦ по горизонтали и вертикали
// CS, DATA, CLK - номера пинов

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

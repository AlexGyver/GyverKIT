/*
    Лёгкая библиотека двухмерной графики для дисплеев и матриц
    Документация:
    GitHub: https://github.com/GyverLibs/GyverGFX
    Возможности:
    - Точки
    - Линии
    - Прямоугольники
    - Скруглённые прямоугольники
    - Круги
    - Кривая Безье
    - Битмап
    - Вывод текста (русский, английский) нескольких размеров
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
*/

#ifndef GyverGFX_h
#define GyverGFX_h
#include <Arduino.h>
#include <Print.h>
#include "charMap.h"

#define GFX_CLEAR 0
#define GFX_FILL 1
#define GFX_STROKE 2
#define GFX_ADD 0
#define GFX_REPLACE 1

#define GFX_SWAP(x,y) if (x > y) x = x ^ y ^ (y = x);

class GyverGFX : public Print {
public:
    GyverGFX(int x, int y);
    virtual void dot(int x, int y, uint8_t fill = 1);                           // точка
    virtual void fastLineH(int y, int x0, int x1, uint8_t fill = 1);            // вертикальная линия
    virtual void fastLineV(int x, int y0, int y1, uint8_t fill = 1);            // горизонтальная линия
    virtual void line(int x0, int y0, int x1, int y1, uint8_t fill = 1);        // линия
    virtual void rect(int x0, int y0, int x1, int y1, uint8_t fill = 1);        // прямоугольник
    virtual void roundRect(int x0, int y0, int x1, int y1, uint8_t fill = 1);   // скруглённый прямоугольник
    virtual void circle(int x, int y, int radius, uint8_t fill = 1);            // окружность
    virtual void bezier(uint8_t* arr, uint8_t size, uint8_t dense, uint8_t fill = 1);   // кривая Безье
    virtual void bezier16(int* arr, uint8_t size, uint8_t dense, uint8_t fill = 1);     // кривая Безье 16 бит. fill - GFX_CLEAR/GFX_FILL/GFX_STROKE
    virtual void drawBitmap(int x, int y, const uint8_t *frame, int width, int height, uint8_t invert = 0, byte mode = 0);  // битмап

    void setCursor(int x, int y);           // установить курсор
    void setScale(uint8_t scale);           // масштаб текста
    void invertText(bool inv);              // инвертировать текст
    void autoPrintln(bool mode);            // автоматический перенос строки
    void textDisplayMode(bool mode);        // режим вывода текста GFX_ADD/GFX_REPLACE
    virtual size_t write(uint8_t data);
    using Print::write;

private:
    uint8_t getFont(uint8_t font, uint8_t row);
    int _x = 0, _y = 0;
    uint8_t _scaleX = 1, _scaleY = 8;
    bool _invert = 0, _println = 0, _mode = 1;
    uint8_t _lastChar;
    int _maxX = 5, _maxY = 5;
};
#endif

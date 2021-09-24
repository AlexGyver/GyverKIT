/*
    Cамая резкая библиотека для матриц MAX7219 на диком западе
    Документация:
    GitHub: https://github.com/GyverLibs/GyverMAX7219
    Возможности:
    - Наследует графику из GyverGFX
    - Подключение матриц зигзагом
    - Аппаратный и программный SPI
    - Невероятная оптимизация
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - оптимизирован SPI
    v1.2 - переделан FastIO
    v1.2.1 - исправлен баг в SPI (с 1.2)
    v1.2.2 - убран FastIO
    v1.3 - мелкие доработки и оптимизация, добавил поворот матриц
*/

#ifndef GyverMAX7219_h
#define GyverMAX7219_h

#include <Arduino.h>
#include <SPI.h>
#include <GyverGFX.h>

#ifndef MAX_SPI_SPEED
#define MAX_SPI_SPEED 1000000
#endif

static SPISettings MAX_SPI_SETT(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);

template < uint8_t width, uint8_t height, uint8_t CSpin, uint8_t DATpin = 0, uint8_t CLKpin = 0 >
class MAX7219 : public GyverGFX {
public:
    MAX7219() : GyverGFX(width * 8, height * 8) {
        begin();
    }
    
    // запустить
    void begin() {
        pinMode(CSpin, OUTPUT);
        if (DATpin == CLKpin) {
            SPI.begin();			
        } else {			
            pinMode(DATpin, OUTPUT);
            pinMode(CLKpin, OUTPUT);
        }		
        sendCMD(0x0f, 0x00);  // отключить режим теста
        sendCMD(0x09, 0x00);  // выключить декодирование
        sendCMD(0x0a, 0x00);  // яркость
        sendCMD(0x0b, 0x0f);  // отображаем всё
        sendCMD(0x0C, 0x01);  // включить
        clearDisplay();       // очистить
    }
    
    // установить яркость [0-15]
    void setBright(uint8_t value) {	// 8x8: 0/8/15 - 30/310/540 ma
        sendCMD(0x0a, value);		// яркость 0-15
    }
    
    // переключить питание
    void setPower(bool value) {
        sendCMD(0x0c, value);
    }
    
    // очистить
    void clear() {
        fillByte(0);
    }
    
    // залить
    void fill() {
        fillByte(255);
    }
    
    // залить байтом
    void fillByte(uint8_t data) {
        for (int i = 0; i < width * height * 8; i++) buffer[i] = data;      
    }
    
    // установить точку
    void dot(int x, int y, uint8_t fill = 1) {
        int pos = getPosition(x, y);
        if (pos >= 0) bitWrite(buffer[pos], _bx, fill);
    }

    // получить точку
    bool get(int x, int y) {
        int pos = getPosition(x, y);
        if (pos >= 0) return bitRead(buffer[pos], _bx);
        else return 0;
    }

    // обновить
    void update() {
        int count = 0;
        for (int k = 0; k < 8; k++) {
            beginData();
            for (int i = 0; i < _amount; i++) sendData(8 - k, buffer[count++]);
            endData();
        }
    }
    
    // начать отправку
    void beginData() {
        if (DATpin == CLKpin) SPI.beginTransaction(MAX_SPI_SETT);
        fastWrite(CSpin, 0);		
    }
    
    // закончить отправку
    void endData() {		
        fastWrite(CSpin, 1);
        if (DATpin == CLKpin) SPI.endTransaction();
    }
    
    // отправка данных напрямую в матрицу (строка, байт)
    void sendByte(uint8_t address, uint8_t value) {
        beginData();
        sendData(address + 1, value);
        endData();
    }
    
    // очистить дисплей (не буфер)
    void clearDisplay() {
        for (int k = 0; k < 8; k++) {
            beginData();
            for (int i = 0; i < _amount; i++) sendData(8 - k, 0);
            endData();
        }
    }
    
    // поворот матриц (0, 1, 2, 3 на 90 град по часовой стрелке)
    void setRotation(uint8_t rot) {
        _rot = rot;
    }

    uint8_t buffer[width * height * 8];

private:
    int getPosition(int x, int y) {
        if (x >= 0 && x < width * 8 && y >= 0 && y < height * 8) {
            int b = y;
            switch (_rot) {
            case 1:
                y = (y & 0xF8) + (x & 7);
                x = (x & 0xF8) + 7 - (b & 7);
                break;
            case 2:
                x = (x & 0xF8) + 7 - (x & 7);   // (x / 8 + 1) * 8 - 1 - (x % 8)
                y = (y & 0xF8) + 7 - (y & 7);   // (y / 8 + 1) * 8 - 1 - (y % 8)
                break;
            case 3:
                y = (y & 0xF8) + 7 - (x & 7);
                x = (x & 0xF8) + (b & 7);
                break;
            }
            if ((y >> 3) & 1) {               	// если это нечётная матрица: (y / 8) % 2
                x = width * 8 - 1 - x;          // отзеркалить x
                y = (y & 0xF8) + (7 - (y & 7)); // отзеркалить y: (y / 8) * 8 + (7 - (y % 8));
            }
            _bx = x & 7;
            return width * (height - 1 - (y >> 3)) + (width - 1 - (x >> 3)) + (y & 7) * width * height; // позиция в буфере
        }
        return -1;
    }
    
    void fastWrite(const uint8_t pin, bool val) {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
        if (pin < 8) bitWrite(PORTD, pin, val);
        else if (pin < 14) bitWrite(PORTB, (pin - 8), val);
        else if (pin < 20) bitWrite(PORTC, (pin - 14), val);
#elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
        bitWrite(PORTB, pin, val);
#else
        digitalWrite(pin, val);
#endif
    }
    
    void F_fastShiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t data) {
#if defined(AVR)
        volatile uint8_t *_clk_port = portOutputRegister(digitalPinToPort(clockPin));
        volatile uint8_t *_dat_port = portOutputRegister(digitalPinToPort(dataPin));
        uint8_t _clk_mask = digitalPinToBitMask(clockPin);
        uint8_t _dat_mask = digitalPinToBitMask(dataPin);
        for (uint8_t i = 0; i < 8; i++)  {
            if (bitOrder == MSBFIRST) {
                if (data & (1 << 7)) *_dat_port |= _dat_mask;
                else *_dat_port &= ~_dat_mask;
                data <<= 1;
            } else {
                if (data & 1) *_dat_port |= _dat_mask;
                else *_dat_port &= ~_dat_mask;
                data >>= 1;
            }
            *_clk_port |= _clk_mask;
            *_clk_port &= ~_clk_mask;
        }
#else
        shiftOut(dataPin, clockPin, bitOrder, data);
#endif

    }
    void sendData(uint8_t address, uint8_t value) {
        if (DATpin == CLKpin) {
            SPI.transfer(address);
            SPI.transfer(value);
        } else {
            F_fastShiftOut(DATpin, CLKpin, MSBFIRST, address);
            F_fastShiftOut(DATpin, CLKpin, MSBFIRST, value);
        }		
    }
    void sendCMD(uint8_t address, uint8_t value) {
        beginData();
        for (int i = 0; i < _amount; i++) sendData(address, value);
        endData();
    }
    
    const int _amount = width * height;
    int _row = 0, _count = 0;
    uint8_t _rot = 0, _bx = 0;
};
#endif

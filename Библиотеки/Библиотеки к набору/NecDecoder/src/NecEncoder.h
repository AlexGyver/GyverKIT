#ifndef _NecEncoder_h
#define _NecEncoder_h
#include <Arduino.h>

// отправка
class NecEncoder {
public:
    NecEncoder(uint8_t pin, uint8_t del = 10) : _pin(pin), _del(del) {
        pinMode(pin, OUTPUT);
    }
    
    void send(uint8_t addr, uint8_t cmd) {
        pulse38(9000);
        delayMicroseconds(4500);
        sendData(addr);
        sendData(~addr);
        sendData(cmd);
        sendData(~cmd);
        pulse38(562);
    }

private:
    void pulse38(int dur) {
        dur = (dur / 13) & ~1;   // кол-во пакетов по 13мкс + округляем нечетного
        bool flag = 1;
        for (int i = 0; i < dur; i++) {
            digitalWrite(_pin, flag);
            flag = !flag;
            delayMicroseconds(_del);
        }
    }

    void sendData(uint8_t data) {
        int i = 8;
        while (i--) {
            pulse38(562);
            if (data & (1 << 7)) delayMicroseconds(1687);
            else delayMicroseconds(562);
            data <<= 1;
        }
    }
    
    const uint8_t _pin, _del;
};
#endif
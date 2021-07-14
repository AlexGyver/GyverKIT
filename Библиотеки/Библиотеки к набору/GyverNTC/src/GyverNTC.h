/*
    Библиотека для работы с NTC термисторами по закону Стейнхарта-Харта
    Документация: 
    GitHub: https://github.com/GyverLibs/GyverNTC
    Возможности:
    - Установка параметров термистора
    - Получение температуры
    - Встроенное усреднение
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - небольшая оптимизация, повышение точности
*/

#ifndef GyverNTC_h
#define GyverNTC_h
#include <Arduino.h>
#define _T_SAMPLE_AVERAGE 20   // количество чтений для усреднения

class GyverNTC {
public:
    GyverNTC(byte pin, int resistance, int beta, int tempBase = 25, int resistBase = 10000) :
    _pin(pin), _resistance(resistance), _beta(beta), _tempBase(tempBase), _resistBase(resistBase) {}
    
    // прочитать температуру с пина
    float getTemp() {
        return computeTemp(analogRead(_pin));
    }
    
    // прочитать усреднённую температуру с пина
    float getTempAverage() {
        uint16_t aver = 0;
        for (byte i = 0; i < _T_SAMPLE_AVERAGE; i++) aver += analogRead(_pin);
        return computeTemp((float)aver / _T_SAMPLE_AVERAGE);
    }
    
    // получить температуру из сигнала АЦП (10 бит, float)
    float computeTemp(float analog) {
        analog = (float)_resistBase / _resistance / (1023.0f / analog - 1.0);
        analog = (log(analog) / _beta) + 1.0f / (_tempBase + 273.15f);
        return (1.0f / analog - 273.15);
    }
    
private:    
    const byte _pin = 0;
    const int _beta = 0;
    const int _tempBase = 25;
    const int _resistance = 10000;
    const int _resistBase = 10000;
};
#endif
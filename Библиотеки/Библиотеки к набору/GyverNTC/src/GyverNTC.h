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
        int analogAverage = 0;
        for (byte i = 0; i < _T_SAMPLE_AVERAGE; i++)
        analogAverage += analogRead(_pin);
        analogAverage /= (int)_T_SAMPLE_AVERAGE;
        return computeTemp(analogAverage);
    }
    
    // получить температуру из 10 бит сигнала АЦП
    float computeTemp(int analog) {
        float temp;
        temp = _resistBase / ((float)1024 / analog - 1);
        temp /= (float)_resistance;                 // (R/Ro)
        temp = log(temp) / _beta;            		// 1/B * ln(R/Ro)
        temp += (float)1.0 / (_tempBase + 273.15);  // + (1/To)
        temp = (float)1.0 / temp - 273.15;    		// инвертируем и конвертируем в градусы по Цельсию
        return temp;
    }
    
private:    
    const byte _pin = 0;
    const int _beta = 0;
    const int _tempBase = 25;
    const int _resistance = 10000;
    const int _resistBase = 10000;
};
#endif
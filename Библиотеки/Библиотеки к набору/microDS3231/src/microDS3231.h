/*
    Лёгкая библиотека для работы с RTC DS3231 для Arduino
    Документация: 
    GitHub: https://github.com/GyverLibs/microDS3231
    Возможности:
    - Чтение и запись времени
    - Вывод в char* и String
    - Чтение температуры датчика
    
    Egor 'Nich1con' Zakharov & AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.2 - добавлены ограничения на вводимые в setTime числа. Также нельзя ввести 29 февраля увы =)
    v1.3 - пофикшено зависание, когда модуль отключен но опрашивается
    v1.4 - незначительный фикс
    v2.0 - новые возможности, оптимизация и облегчение
    v2.1 - добавил вывод температуры, вывод в String и char
    v2.2 - исправлены дни недели (пн-вс 1-7)
    v2.3 - небольшие исправления, оптимизация, изменён порядок вывода даты
    v2.4 - исправлена установка времени компиляции
*/

#ifndef microDS3231_h
#define microDS3231_h
//#include <microWire.h>	// выбор между библиотеками Wire и microWire
#include <Wire.h>

#include <Arduino.h>
#define COMPILE_TIME F(__TIMESTAMP__)

struct DateTime {
    uint8_t second; 
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint16_t year;
};

class MicroDS3231 {
public:
    MicroDS3231(uint8_t addr = 0x68);               // конструктор. Можно передать адрес    
    void setTime(const __FlashStringHelper* stamp);	// установка времени == времени компиляции
    void setTime(DateTime time);	                // установить из структуры DateTime
    void setTime(int8_t seconds, int8_t minutes, int8_t hours, int8_t date, int8_t month, int16_t year);	// установка времени
    void setHMSDMY(int8_t hours, int8_t minutes, int8_t seconds, int8_t date, int8_t month, int16_t year);	// установка времени тип 2
    
    DateTime getTime(void);			// получить в структуру DateTime
    uint8_t getSeconds(void);		// получить секунды
    uint8_t getMinutes(void);		// получить минуты
    uint8_t getHours(void);			// получить часы
    uint8_t getDay(void);			// получить день недели
    uint8_t getDate(void);			// получить число
    uint16_t getYear(void);			// получить год
    uint8_t getMonth(void);			// получить месяц
    
    String getTimeString();			// получить время как строку вида HH:MM:SS
    String getDateString();			// получить дату как строку вида DD.MM.YYYY
    void getTimeChar(char* array);	// получить время как char array [8] вида HH:MM:SS
    void getDateChar(char* array);	// получить дату как char array [10] вида DD.MM.YYYY       
    
    bool lostPower(void);			// проверка на сброс питания
    float getTemperatureFloat(void);// получить температуру float
    int getTemperature(void);		// получить температуру int
    
private:
    uint8_t encodeRegister(int8_t data);
    int getTemperatureRaw(void);
    uint8_t readRegister(uint8_t addr);
    uint8_t unpackRegister(uint8_t data);
    uint8_t unpackHours(uint8_t data);
    const uint8_t _addr;
};

#endif
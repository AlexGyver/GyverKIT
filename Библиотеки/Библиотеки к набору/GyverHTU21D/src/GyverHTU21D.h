/*
    Лёгкая библиотека для датчика температуры и влажности HTU21D
    Документация: 
    GitHub: https://github.com/GyverLibs/GyverHTU21D
    Возможности:
    - Асинхронное чтение
    - Блокирующее чтение
    - Установка разрешения
    - Проверка напряжения
    - Управление нагревателем
    
    Nich1con, AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
*/

#ifndef _GyverHTU21D_h
#define _GyverHTU21D_h

#include <Arduino.h>
#include <Wire.h>

/* Команды управления HTU21D */
#define HTU21D_IIC_ADDR         0x40
#define HTU21D_RESET_CMD        0xFE
#define HTU21D_CFG_READ_CMD     0xE7
#define HTU21D_CFG_WRITE_CMD    0xE6
#define HTU21D_TRIG_T_CMD       0xF3
#define HTU21D_TRIG_H_CMD       0xF5
#define HTU21D_TRIG_T_HOLD_CMD  0xE3
#define HTU21D_TRIG_H_HOLD_CMD  0xE5

/* Варианты разрешения датчика */
#define HTU21D_RES_HIGH     0b00000000 // RH = 12Bit, T = 14Bit
#define HTU21D_RES_MEDIUM   0b10000001 // RH = 11Bit, T = 11Bit
#define HTU21D_RES_LOW      0b00000001 // RH = 8Bit, T = 12Bit

class GyverHTU21D {
public:
    // запуск и проверка наличия датчика
    bool begin(void) {
        Wire.begin();
        bufH = bufT = 300;  // сброс
        return request(HTU21D_RESET_CMD);
    }
    
    // Установка разрешения
    void setResolution(uint8_t mode) {
        uint8_t reg = readCfgReg() & 0b01111110;
        writeCfgReg(reg | mode);
    }
    
    // Включение / выключение подогрева
    void setHeater(bool state) {
        uint8_t reg = readCfgReg() & 0b11111011;
        writeCfgReg(reg | (state ? 0b100 : 0b000));
    }
    
    // Состояние питания
    bool powerGood(void) {
        return (readCfgReg() & 0b01000000);
    }

    // Запросить температуру
    bool requestTemperature(void) {
        return request(HTU21D_TRIG_T_CMD);
    }
    
    // Запросить влажность
    bool requestHumidity(void) {
        return request(HTU21D_TRIG_H_CMD);
    }
    
    // Прочитать температуру
    bool readTemperature(void) {
        uint16_t data = readValue();
        if (data == 0xffff) return 0;
        else bufT = (0.002681f * data - 46.85f);
        return 1;
    }
    
    // Прочитать влажность
    bool readHumidity(void) {
        uint16_t data = readValue();
        if (data == 0xffff) return 0;
        else bufH = ((0.001907 * (data ^ 0x02)) - 6);
        return 1;
    }
    
    // Получить температуру
    float getTemperature(void) {
        return bufT;
    }
    
    // Получить влажность
    float getHumidity(void) {
        return bufH;
    }
    
    // прочитать температуру (блокирующая функция)
    float getTemperatureWait() {
        request(HTU21D_TRIG_T_HOLD_CMD);
        while(!digitalRead(SCL));
        readTemperature();
        return bufT;
    }
    
    // прочитать флажность (блокирующая функция)
    float getHumidityWait() {
        request(HTU21D_TRIG_H_HOLD_CMD);
        while(!digitalRead(SCL));
        readHumidity();
        return bufH;
    }
    
    // асинхронный тикер чтения, true если готово. Можно указать свой период
    bool readTick(uint16_t prd = 100) {
        if (millis() - tmr >= prd) {
            tmr = millis();
            if (mode) {
                readTemperature();
                requestHumidity();
            } else {
                readHumidity();
                requestTemperature();
            }
            mode = !mode;
            return (bufH != 300 && bufT != 300);    // пропуск непрочитанных
        }
        return 0;
    }

private:
    float bufH = 300, bufT = 300;
    uint32_t tmr = 0;
    uint8_t mode = 0;
    
    uint16_t readValue() {
        Wire.requestFrom(HTU21D_IIC_ADDR, 3);
        uint16_t data = (Wire.read() << 8) | Wire.read();
        if (checkCRC(data) != Wire.read()) data = 0xffff;
        return data;
    }
    
    // запрос по команде
    bool request(uint8_t cmd) {
        Wire.beginTransmission(HTU21D_IIC_ADDR);    // Начинаем передачу
        Wire.write(cmd);                            // Отправляем команду
        return (!Wire.endTransmission());
    }
    
    // Запись регистра конфигурации
    void writeCfgReg(uint8_t reg) {
        Wire.beginTransmission(HTU21D_IIC_ADDR);
        Wire.write(HTU21D_CFG_WRITE_CMD);
        Wire.write(reg);
        Wire.endTransmission();
    }
    
    // Чтение регистра конфигурации
    uint8_t readCfgReg(void) {
        Wire.beginTransmission(HTU21D_IIC_ADDR);
        Wire.write(HTU21D_CFG_WRITE_CMD);
        if (Wire.endTransmission()) return;
        Wire.requestFrom(HTU21D_IIC_ADDR, 1);
        return Wire.read();
    }

    // Расчет CRC по полиному HTU21D
    uint8_t checkCRC(uint16_t data) {
        for (uint8_t i = 0; i < 16; i++) {
            if (data & 0x8000) data = (data << 1) ^ 0x13100;
            else data <<= 1;
        }
        return (data >> 8);
    }
};
#endif
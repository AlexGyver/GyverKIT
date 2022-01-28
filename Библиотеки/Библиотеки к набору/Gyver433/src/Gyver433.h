/*
    Библиотека для радиомодулей 433 МГц и Arduino
    Документация: 
    GitHub: https://github.com/GyverLibs/Gyver433
    Возможности:
    - Поддержка кривых китайских модулей
    - Встроенный CRC контроль целостности
    - Ускоренный алгоритм IO для AVR Arduino
    - Асинхронный приём в прерывании
    - Супер лёгкая либа, заведётся даже на тини13
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - оптимизация, новый интерфейс, поддержка дешёвых синих модулей, работа в прерывании
    v1.2 - улучшение качества связи, оптимизация работы в прерывании
    v1.3 - добавлен вывод RSSI
    v1.4 - переделан FastIO
    v1.4.1 - убран FastIO, CRC вынесен отдельно
    v2.0 - убран буфер на отправку, убран манчестер, полностью переделан и оптимизирован интерфейс связи
    v2.0.1 - fix compiler warnings
*/

#ifndef _Gyver433_h
#define _Gyver433_h
#include <Arduino.h>
#include "G433_crc.h"

#define TRAINING_TIME_SLOW (500000)         // время синхронизации для SLOW_MODE

// =========================================================================
#ifndef G433_SPEED
#define G433_SPEED 2000
#endif

#ifndef G433_RSSI_COUNT
#define G433_RSSI_COUNT 8
#endif

// тайминги интерфейса
#define G433_HIGH (1000000ul / G433_SPEED)  // время HIGH
#define G433_LOW (G433_HIGH / 2)            // время LOW
#define G433_START (G433_HIGH * 2)          // стартовый импульс
#define G433_TRAIN (G433_HIGH * 3 / 2)      // синхроимпульс

#define G433_WINDOW (G433_HIGH / 4)
#define G433_EDGE_L (G433_LOW - G433_WINDOW)
#define G433_EDGE_LH (G433_HIGH - G433_WINDOW)
#define G433_EDGE_HT (G433_HIGH + G433_WINDOW)
#define G433_EDGE_TS (G433_TRAIN + G433_WINDOW)
#define G433_EDGE_S (G433_START + G433_WINDOW)

// жоский delay для avr
#ifdef _delay_us
#define G433_DELAY(x) _delay_us(x)
#else
#define G433_DELAY(x) delayMicroseconds(x)
#endif

// режимы CRC
#define G433_NOCRC 0
#define G433_CRC8 1
#define G433_XOR 2

// количество синхроимпульсов
#if defined(G433_FAST)
#define TRAINING_TIME 10000
#elif defined(G433_MEDIUM)
#define TRAINING_TIME 100000
#elif defined(G433_SLOW)
#define TRAINING_TIME (TRAINING_TIME_SLOW)
#else
#define TRAINING_TIME 100000
#endif

// =================================== ПЕРЕДАТЧИК ===================================
template <uint8_t TX_PIN, uint8_t CRC_MODE = G433_CRC8>
class Gyver433_TX {
public:
    Gyver433_TX() {
        pinMode(TX_PIN, OUTPUT);
    }
    
    // отправка, блокирующая. Кушает любой тип данных
    template <typename T>
    void sendData(T &data) {
        uint8_t *ptr = (uint8_t*) &data;
        write(ptr, sizeof(T));
    }
    
    // отправка сырого набора байтов
    void write(uint8_t* buf, uint16_t size) {
        uint8_t crc;
        if (CRC_MODE == G433_CRC8) crc = G433_crc8(buf, size);
        else if (CRC_MODE == G433_XOR) crc = G433_crc_xor(buf, size);
        
        // раскачка радио
        flag = 0;
        for (uint16_t i = 0; i < (TRAINING_TIME / G433_TRAIN / 2) * 2 + 1; i++) {
            fastWrite(TX_PIN, flag = !flag);
            G433_DELAY(G433_TRAIN);
        }
        
        // старт бит
        fastWrite(TX_PIN, 0);       // старт
        G433_DELAY(G433_START);     // ждём
        fastWrite(TX_PIN, 1);       // старт бит
        
        for (uint16_t i = 0; i < size; i++) write(buf[i]);  // дата
        if (CRC_MODE) write(crc);               // CRC
        G433_DELAY(G433_TRAIN);
        fastWrite(TX_PIN, flag = !flag);        // стоп
    }

    // отправить байт (без старт бита!)
    void write(uint8_t data) {
        for (uint8_t b = 0; b < 8; b++) {
            if (data & 1) G433_DELAY(G433_HIGH);
            else G433_DELAY(G433_LOW);
            fastWrite(TX_PIN, flag = !flag);
            data >>= 1;
        }
    }
    
private:
    // быстрый IO
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
    
    bool flag = 0;
};

// =================================== ПРИЁМНИК ===================================
template <uint8_t RX_PIN, uint16_t RX_BUF = 64, uint8_t CRC_MODE = G433_CRC8>
class Gyver433_RX {
public:   
    Gyver433_RX() {
        pinMode(RX_PIN, INPUT);
    }
    
    // неблокирующий приём. Вернёт количество успешно принятых байт
    uint16_t tick() {
        if (pinChanged()) tickISR();
        return gotData();
    }
    
    // блокирующий приём. Вернёт количество успешно принятых байт
    uint16_t tickWait() {
        do {
            if (pinChanged()) tickISR();
        } while (state == 1);
        return gotData();
    }
        
    // ручной приём в прерывании по CHANGE. Вернёт 1 (начало приёма), 2 (принят байт), 3 (конец пакета)
    // принятый байт можно прочитать в byteBuf
    uint8_t tickISRraw() {
        uint32_t pulse = micros() - tmr;                    // время импульса
        tmr += pulse;                                       // сброс таймера. Равносильно tmr = micros()
        if (pulse <= G433_EDGE_L) return parse = 0;         // импульс слишком короткий
        trains <<= 1;                                       // счётчик train импульсов (0b11111111 << 1)
        if (pulse <= G433_EDGE_HT && parse) {               // окно LOW/HIGH и идёт парсинг
            byteBuf >>= 1;                                  // двигаем байт-буфер
            if (pulse > G433_EDGE_LH) byteBuf |= (1 << 7);  // пишем бит, если это HIGH
            if (!(++bits & 0x7)) return 2;                  // 2: ПРИНЯТ БАЙТ: собрали байт (каждые 8 бит, 0x7 == 0b111)
        } else if (pulse <= G433_EDGE_TS) {                 // окно START
            trains |= 1;                                    // добавляем 1 справа к trains
            if (parse) {                                    // был парсинг, а это стоп бит
                parse = 0;                                  // стоп машина
                return 3;                                   // 3: ПРИНЯТ ПАКЕТ: принят стоп-бит
            }
        } else if (pulse <= G433_EDGE_S) {                  // окно STOP/TRAINING
            if (trains == 0xFE) {                           // было 7 train импульсов (0xFE == 0b11111110)
                bits = 0;                                   // прерываем парсинг, если он был
                parse = 1;                                  // старт бит, начинаем парсинг
                return 1;                                   // 1: СТАРТ ПРИЁМА
            }
        } else return parse = 0;                            // слишком длинный импульс, выходим
        return 0;
    }
    
    // тикер приёма для вызова в прерывании по CHANGE
    void tickISR() {
        switch (tickISRraw()) {
        case 1:                // СТАРТ БИТ
            if (!state) {      // старта не было
                state = 1;     // старт
                bytes = 0;     // сброс
            }                  // старт был - ошибка приёма
            break;
        case 2:                                 // ПРИНЯТ БАЙТ
            if (state == 1) {                   // парсинг идёт
                buffer[bytes] = byteBuf;        // пишем в буфер
                bytes++;                        // счётчик принятых
                if (bytes > sizeof(buffer)) {   // буфер переполнен
                    state = 3;                  // флаг на чтение
                }
            }
            break;
        case 3:                           // КОНЕЦ ПАКЕТА
            if (state == 1) state = 2;    // флаг на чтение
            break;
        }
    }
    
    // если пакет прочитан успешно - вернёт количество байт в нём
    uint16_t gotData() {
        if (state >= 2) {                                   // флаг на чтение
            size = 0;                                       // обнуляем размер
            if (state != 3 && bytes != 0) {                 // если буфер не переполнен, проверяем CRC
                if (CRC_MODE == G433_CRC8) {                // CRC8 
                    if (!G433_crc8(buffer, bytes)) size = bytes - 1;
                } else if (CRC_MODE == G433_XOR) {          // CRC XOR
                    if (!G433_crc_xor(buffer, bytes)) size = bytes - 1;
                } else size = bytes;                        // без CRC
            }
        #ifndef G433_CUT_RSSI
            if (!size) errCount++;                          // принято 0 байт - ошибка
            if (++rcCount >= G433_RSSI_COUNT) {
                RSSI = 100 - errCount * 100 / G433_RSSI_COUNT;
                errCount = rcCount = 0;
            }
        #endif
            state = 0;
            return size;            
        }
        return 0;
    }    
    
    // прочитает буфер в любой тип данных
    template <typename T>
    bool readData(T &data) {
        if (sizeof(T) > RX_BUF) return false;   // великовато для буфера
        if (sizeof(T) != size) return false;    // данные не соответствуют
        uint8_t *ptr = (uint8_t*) &data;
        for (uint16_t i = 0; i < sizeof(T); i++) *ptr++ = buffer[i];
        return true;
    }
    
    // получить качество приёма (процент успешных передач)
    uint8_t getRSSI() {
    #ifndef G433_CUT_RSSI
        return RSSI;
    #endif
    }
    
    // получить размер принятых данных
    uint16_t getSize() {
        return size;
    }
    
    // размер принятых данных
    uint16_t size = 0;
    
    // доступ к буферу
    uint8_t buffer[RX_BUF + !!CRC_MODE];
    uint8_t byteBuf;
    
private:
    bool fastRead(const uint8_t pin) {
    #if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
        if (pin < 8) return bitRead(PIND, pin);
        else if (pin < 14) return bitRead(PINB, pin - 8);
        else if (pin < 20) return bitRead(PINC, pin - 14);
    #elif defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny13__)
        return bitRead(PINB, pin);
    #else
        return digitalRead(pin);
    #endif
        return 0;
    }

    bool pinChanged() {
        if (prevBit != fastRead(RX_PIN)) {
            prevBit = !prevBit;
            return 1;
        } return 0;
    }

    bool prevBit;
    volatile uint8_t state = 0;
    volatile uint8_t parse = 0, trains = 0;
    volatile uint32_t tmr = 0;
    uint8_t bits = 0, bytes = 0;
#ifndef G433_CUT_RSSI
    uint8_t errCount = 0, rcCount = 0, RSSI = 100;
#endif
};

#endif
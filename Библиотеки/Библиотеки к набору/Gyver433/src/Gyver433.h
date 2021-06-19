/*
    Библиотека для радиомодулей 433 МГц и Arduino
    Документация: 
    GitHub: https://github.com/GyverLibs/Gyver433
    Возможности:
    - Супер лёгкая либа, заведётся даже на тини13 (отправка)
    - Поддержка кривых китайских модулей
    - Интерфейс Manchester или Pulselength
    - Встроенный CRC контроль целостности (CRC8 или XOR)
    - Ускоренный алгоритм IO для AVR Arduino
    - Опционально работа в прерывании (приём данных)
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - оптимизация, новый интерфейс, поддержка дешёвых синих модулей, работа в прерывании
    v1.2 - улучшение качества связи, оптимизация работы в прерывании
*/

#ifndef Gyver433_h
#define Gyver433_h
#include <Arduino.h>
#include "FastIO.h"

uint8_t G433_crc8(uint8_t *buffer, uint8_t size);       // ручной CRC8
uint8_t G433_crc_xor(uint8_t *buffer, uint8_t size);    // ручной CRC XOR
#define TRAINING_TIME_SLOW (500000ul)                   // время синхронизации для SLOW_MODE

// =========================================================================
#ifndef G433_SPEED
#define G433_SPEED 2000
#endif

// тайминги интерфейса
#define FRAME_TIME (1000000ul / G433_SPEED) // время фрейма (либо HIGH)
#define HALF_FRAME (FRAME_TIME / 2)         // полфрейма (либо LOW)
#define START_PULSE (FRAME_TIME * 2)        // стартовый импульс

// окно времени для обработки импульса
#define START_MIN (START_PULSE * 3 / 4)
#define START_MAX (START_PULSE * 5 / 4)
#define FRAME_MIN (FRAME_TIME * 3 / 4)
#define FRAME_MAX (FRAME_TIME * 5 / 4)
#define HALF_FRAME_MIN (HALF_FRAME * 3 / 4)
#define HALF_FRAME_MAX (HALF_FRAME * 5 / 4)

// жоский delay для avr
#ifdef AVR
#define G433_DELAY _delay_us
#else
#define G433_DELAY delayMicroseconds
#endif

// режимы CRC
#define G433_NOCRC 0
#define G433_CRC8 1
#define G433_XOR 2

// количество синхроимпульсов
#if defined(G433_FAST)
#define TRAINING_PULSES 10
#elif defined(G433_MEDIUM)
#define TRAINING_PULSES 50
#elif defined(G433_SLOW)
#define TRAINING_PULSES (TRAINING_TIME_SLOW / FRAME_TIME / 2)
#else
#define TRAINING_PULSES 50
#endif

// crc8 один байт
void G433_crc8_byte(uint8_t &crc, uint8_t data);

// ============ ПЕРЕДАТЧИК ============
template <uint8_t TX_PIN, uint16_t TX_BUF = 64, uint8_t CRC_MODE = G433_CRC8>
class Gyver433_TX {
public:
    Gyver433_TX() {
        pinMode(TX_PIN, OUTPUT);
    }
    
    // отправка, блокирующая. Кушает любой тип данных
    template <typename T>
    bool sendData(T &data) {
        if (sizeof(T) > TX_BUF) return 0;
        const uint8_t *ptr = (const uint8_t*) &data;
        for (uint16_t i = 0; i < sizeof(T); i++) buffer[i] = *ptr++;
        if (CRC_MODE == G433_CRC8) {
            buffer[sizeof(T)] = G433_crc8(buffer, sizeof(T));
            write(buffer, sizeof(T) + 1);
        } else if (CRC_MODE == G433_XOR) {
            buffer[sizeof(T)] = G433_crc_xor(buffer, sizeof(T));
            write(buffer, sizeof(T) + 1);
        } else {
            write(buffer, sizeof(T));
        }
        return 1;
    }
    
    // отправка сырого набора байтов
    void write(uint8_t* buf, uint16_t size) {
        for (uint16_t i = 0; i < TRAINING_PULSES; i++) {
            fastWrite(TX_PIN, 1);
            G433_DELAY(FRAME_TIME);
            fastWrite(TX_PIN, 0);
            G433_DELAY(FRAME_TIME);
        }
        fastWrite(TX_PIN, 1);       // старт
        G433_DELAY(START_PULSE);    // ждём
        fastWrite(TX_PIN, 0);       // старт бит
        
        #ifdef G433_MANCHESTER
        G433_DELAY(HALF_FRAME);       // ждём
        for (uint16_t n = 0; n < size; n++) {
            uint8_t data = buf[n];
            for (uint8_t b = 0; b < 8; b++) {
                fastWrite(TX_PIN, !(data & 1));
                G433_DELAY(HALF_FRAME);
                fastWrite(TX_PIN, (data & 1));                
                G433_DELAY(HALF_FRAME);
                data >>= 1;
            }
        }
        fastWrite(TX_PIN, 0);   // конец передачи
        #else
        bool flag = 0;
        for (uint16_t n = 0; n < size; n++) {
            uint8_t data = buf[n];
            for (uint8_t b = 0; b < 8; b++) {
                if (data & 1) G433_DELAY(FRAME_TIME);
                else G433_DELAY(HALF_FRAME);
                fastWrite(TX_PIN, flag = !flag);
                data >>= 1;
            }
        }
        #endif        
    }
    
    // доступ к буферу
    uint8_t buffer[TX_BUF + !!CRC_MODE];
    
private:    
};

// ============ ПРИЁМНИК ============
template <uint8_t RX_PIN, uint16_t RX_BUF = 64, uint8_t CRC_MODE = G433_CRC8>
class Gyver433_RX {
public:
    Gyver433_RX() {
        pinMode(RX_PIN, INPUT);
    }
    
    // неблокирующий приём, вернёт кол-во успешно принятых байт
    uint16_t tick() {            
        if (pinChanged()) checkState();       
        return gotData();
    }
    
    // tick для вызова в прерывании по CHANGE
    void tickISR() {
        #ifdef G433_MANCHESTER
        if (pinChanged()) checkState();        
        #else
        checkState();
        #endif
    }
    
    // блокирующий приём, вернёт кол-во успешно принятых байт
    uint16_t tickWait() {
        do {
            if (tick()) return size;
        } while (parse == 2);
        return 0;
    }
    
    // прочитает буфер в любой тип данных
    template <typename T>
    bool readData(T &data) {
        if (sizeof(T) > RX_BUF) return false;		
        uint8_t *ptr = (uint8_t*) &data;	
        for (uint16_t i = 0; i < sizeof(T); i++) *ptr++ = buffer[i];
        return true;
    }
    
    // вернёт true при получении корректных данных
    uint16_t gotData() {
        if (parse == 2 && millis() - tmr2 >= 10) {              // фрейм не закрыт
            parse = size = 0;                                   // приём окончен   
            if (byteCount > 1) {                                // если что то приняли
                if (CRC_MODE == G433_CRC8) {                    // CRC8 
                    if (!G433_crc8(buffer, byteCount)) size = byteCount - 2;
                } else if (CRC_MODE == G433_XOR) {              // CRC XOR
                    if (!G433_crc_xor(buffer, byteCount)) size = byteCount - 2;
                } else size = byteCount - 1;                    // без CRC
            }
            return size;
        }
        return 0;
    }
    
    // получить размер принятых данных
    uint16_t getSize() {
        return size;
    }
    
    // размер принятых данных
    uint16_t size = 0;
    
    // доступ к буферу
    uint8_t buffer[RX_BUF + !!CRC_MODE];
    
private:
    bool pinChanged() {
        bit = fastRead(RX_PIN);
        if (bit != prevBit) {
            prevBit = bit;
            return 1;
        } return 0;
    }

    void checkState() {
        uint32_t thisPulse = micros() - tmr;            // время импульса
        if (parse == 1) {   			                // в прошлый раз поймали фронт
            #ifdef G433_MANCHESTER
            tmr += thisPulse;                           // сброс таймера приёма
            #endif
            if (thisPulse > START_MIN && thisPulse < START_MAX) {   // старт бит?
                tmr2 = millis();                                    // сброс таймера активности
                parse = 2;                                          // ключ на старт
                byteCount = bitCount = size = 0;                    // сброс
            } else parse = 0;									    // не старт бит
        } else if (parse == 2) {		                            // идёт парсинг            
            #ifdef G433_MANCHESTER
            if (thisPulse > FRAME_MIN && thisPulse < FRAME_MAX) {   // фронт внутри таймфрейма
                tmr += thisPulse;                                   // синхронизируем тайминги
                tmr2 = millis();                                    // сброс таймера активности
                buffer[byteCount] >>= 1;                            // двигаем байт
                buffer[byteCount] |= (bit << 7);                    // пишем данные
                bitCount++;                                         // счётчик битов                
            }
            #else
            uint8_t state = 2;
            if (thisPulse > HALF_FRAME_MIN && thisPulse < HALF_FRAME_MAX) state = 0; // low бит
            else if (thisPulse > FRAME_MIN && thisPulse < FRAME_MAX) state = 1;      // high бит
            if (state != 2) {                
                buffer[byteCount] >>= 1;                            // двигаем байт
                buffer[byteCount] |= (state << 7);                  // пишем данные
                bitCount++;                                         // счётчик битов
                tmr2 = millis();                                    // сброс таймера активности
            }
            #endif
            if (bitCount == 8) {                        // собрали байт
                bitCount = 0;                           // сброс
                if (++byteCount >= RX_BUF) parse = 0;   // буфер переполнен                
            }            
        }
        #ifdef G433_MANCHESTER
        if (bit && parse == 0) {                        // ловим фронт
            parse = 1;									// флаг на старт
            tmr += thisPulse;                           // сброс таймера
        }        
        #else
        if (parse == 0) parse = 1;                      // принудительно стартуем
        tmr += thisPulse;                               // сброс таймера приёма
        #endif
    }
    bool bit, prevBit;
    uint8_t parse = 0;
    uint32_t tmr = 0, tmr2 = 0;
    uint8_t bitCount = 0, byteCount = 0;
};

// ===== CRC =====
void G433_crc8_byte(uint8_t &crc, uint8_t data) {
#if defined (__AVR__)
    // резкий алгоритм для AVR
    uint8_t counter;
    uint8_t buffer;
    asm volatile (
    "EOR %[crc_out], %[data_in] \n\t"
    "LDI %[counter], 8          \n\t"
    "LDI %[buffer], 0x8C        \n\t"
    "_loop_start_%=:            \n\t"
    "LSR %[crc_out]             \n\t"
    "BRCC _loop_end_%=          \n\t"
    "EOR %[crc_out], %[buffer]  \n\t"
    "_loop_end_%=:              \n\t"
    "DEC %[counter]             \n\t"
    "BRNE _loop_start_%="
    : [crc_out]"=r" (crc), [counter]"=d" (counter), [buffer]"=d" (buffer)
    : [crc_in]"0" (crc), [data_in]"r" (data)
    );
#else
    // обычный для всех остальных
    uint8_t i = 8;
    while (i--) {
        crc = ((crc ^ data) & 1) ? (crc >> 1) ^ 0x8C : (crc >> 1);
        data >>= 1;
    }
#endif
}

uint8_t G433_crc8(uint8_t *buffer, uint8_t size) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < size; i++) G433_crc8_byte(crc, buffer[i]);
    return crc;
}
uint8_t G433_crc_xor(uint8_t *buffer, uint8_t size) {
    uint8_t crc = 0;
    for (uint8_t i = 0; i < size; i++) crc ^= buffer[i];
    return crc;
}
#endif
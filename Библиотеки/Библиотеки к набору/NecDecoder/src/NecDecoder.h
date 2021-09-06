/*
    Лёгкая библиотека для декодирования ИК протокола NEC (Японский вариант)
    Документация:
    GitHub: https://github.com/GyverLibs/NecDecoder
    Возможности:
    - Библиотека не забирает никакие прерывания
    - Отсчет времени на micros()
    - Обработка команды повтора (удержание кнопки пульта)
    - Обработка и чтение занимает 500б Flash
    by Nich1con & AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
    v1.1 - исправлены ошибки, добавлена возможность подключения обработчиков, добавлен контроль потока 
    v2.0 - завёз очень много оптимизации, стабильноси, надёжности, упрощения и удобства (by AlexGyver)
*/

#ifndef _NecDecoder_h
#define _NecDecoder_h
#include <Arduino.h>

#define _NEC_TOLERANCE 150      // допуск high/low, мкс
#define _NEC_TOLERANCE2 1500    // допуск start/repeat, мкс
#define _NEC_SKIP_REPEAT 2      // пропуск первых повторов, шт

// Тайминги NEC, мкс
#define _NEC_HIGH_BIT 	2250
#define _NEC_LOW_BIT	1150
#define _NEC_START_BIT  14400
#define _NEC_REPEAT		12300

// =========================================================================
#define _NEC_HIGH_MIN (_NEC_HIGH_BIT - _NEC_TOLERANCE)
#define _NEC_HIGH_MAX (_NEC_HIGH_BIT + _NEC_TOLERANCE)
#define _NEC_LOW_MIN (_NEC_LOW_BIT - _NEC_TOLERANCE)
#define _NEC_LOW_MAX (_NEC_LOW_BIT + _NEC_TOLERANCE)

#define _NEC_START_MIN (_NEC_START_BIT - _NEC_TOLERANCE2)
#define _NEC_START_MAX (_NEC_START_BIT + _NEC_TOLERANCE2)
#define _NEC_REPEAT_MIN (_NEC_REPEAT - _NEC_TOLERANCE2)
#define _NEC_REPEAT_MAX (_NEC_REPEAT + _NEC_TOLERANCE2)

class NecDecoder {
public:
    void tick();                        // Вызывать при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника в прерывании
    bool available();                   // Возвращает true, если корректный пакет прочитан или повторён (isDecoded() + isRepeated())
    uint32_t readPacket();              // Прочитать пакет целиком (адрес + ~адрес + команда + ~команда)
    uint8_t readAddress();              // Прочитать адрес
    uint8_t readCommand();              // Прочитать команду
    bool isDecoded();                   // Возвращает true, если пакет успешно декодирован
    bool isRepeated();                  // Возвращает true, если принят флаг повтора команды
    
    // сервис
    uint8_t readInvAddress();			// Прочитать только байт с инвертированным адресом
    uint8_t readInvCommand();			// Прочитать только байт с инвертированной командой
    
    // совместимость 1.x
    bool addressIsValid();				// Вернет true если адрес прошел проверку
    bool commandIsValid();				// Вернет true если команда прошла проверку
    bool packetIsValid();				// Вернет true если весь пакет прошел проверку
    
private:
    volatile uint32_t _packet = 0;	    // Буфер последнего принятого пакета (4 байта)
    volatile uint32_t _buffer = 0;		// Буфер текущего принимаемого пакета (4 байта)
    volatile uint32_t _tmr = 0;			// Время предыдущего нисходящего фронта (us)
    volatile int8_t _counter = 32;		// Счетчик бит в принимаемом пакете
    volatile int8_t _repeats = 0;       // счётчик повторов
    volatile bool _start = false;       // старт флаг
    volatile bool _decoded = false;		// Флаг готовности данных к чтению (пакет принят и декодирован)
    volatile bool _repeat = false;		// Флаг повтора (пульт отправил то же самое, что и в прошлый раз)
    volatile bool _parity = false;		// Флаг контроля четности
};
#endif
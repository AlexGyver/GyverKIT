/*
    Лёгкая библиотека для декодирования ИК протокола NEC (Японский вариант)
    Документация:
    GitHub: https://github.com/GyverLibs/NecDecoder
    Возможности:
    - Требует вызова специального метода при отрицательном (FALLING) фронте на пине ИК приемника
    - Отсчет времени базируется на micros()
    - Хранение ОДНОГО последнего декодированного пакета данных в буфере
    - Обработка команды повтора (удержания кнопки пульта)
    
    Nich1con for AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
*/

#ifndef NecDecoder_h
#define NecDecoder_h
#include <Arduino.h>

#define _NEC_TOLERANCE 150		// Допуск таймингов протокола в микросекундах (10-250)

class NecDecoder {
public:
    NecDecoder;
    void tick();				// Вызывается при ОТРИЦАТЕЛЬНОМ (FALLING) фронте на пине ИК приемника, например в прерывании
    bool isDecoded();			// Возвращает true когда очередной пакет декодирован
    bool isRepeated();			// Возвращает true если принят флаг повтора команды
    uint32_t readPacket();		// Прочитать весь пакет целиком (адрес + ~адрес + команда + ~команда)
    uint8_t readAddress();		// Прочитать только байт с адресом
    uint8_t readInvAddress();	// Прочитать только байт с инвертированным адресом
    uint8_t readCommand();		// Прочитать только байт с командой
    uint8_t readInvCommand();	// Прочитать только байт с инвертированной командой
    bool addressIsValid();		// Вернет true если адрес прошел проверку
    bool commandIsValid();		// Вернет true если команда прошла проверку
    bool packetIsValid();		// Вернет true если весь пакет прошел проверку
    
private:
    volatile uint32_t _packet_buffer = 0;
    volatile uint32_t _temp_buffer = 0;
    volatile uint32_t _last_edge = 0;
    volatile uint8_t _remain_counter = 32;
    volatile bool _decoded_flag = false;
    volatile bool _repeat_flag = false;
};
#endif
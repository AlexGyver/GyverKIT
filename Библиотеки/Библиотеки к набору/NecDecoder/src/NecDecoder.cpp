#include "NecDecoder.h"
/*
    NEC пакет: 32 бита, 4 блока по 8 бит, LSB<-MSB
    [START] [адрес] [~адрес] [команда] [~команда]
*/

void NecDecoder::tick(void) {
    uint32_t time = micros() - _tmr;                // получаем время последнего импульса
    _tmr += time;                                   // сбрасываем таймер (== (_tmr = micros() ))
    if (time > 150000) _repeats = -1;               // таймаут повторов
    if (_start && time < _NEC_HIGH_MAX) {           // чтение: дата
        uint8_t mode = 2;
        if (time > _NEC_LOW_MIN && time < _NEC_LOW_MAX) mode = 0;           // LOW бит
        else if (time > _NEC_HIGH_MIN && time < _NEC_HIGH_MAX) mode = 1;    // HIGH бит
        if (mode != 2) {                            // HIGH или LOW
            _buffer = _buffer << 1 | mode;          // пишем в буфер
            if (mode) _parity = !_parity;           // чётность
            if (++_counter == 32) {                 // если приняли 32 бита
                if (_parity) return;                // чётность не совпала - пакет битый
                if (((_buffer >> 8) & _buffer) & 0xFF00FF) return;  // пакет битый
                _packet = _buffer;                  // перемещаем пакет из буфера в дату
                _decoded = true;                    // флаг успешного приема пакета
                _repeats = 0;                       // обнуляем счётчик повторов
                _start = false;                     // чтение окончено
                return;
            }
        }
    }
    if (_repeats != -1 && time > _NEC_REPEAT_MIN && time < _NEC_REPEAT_MAX) {   // чтение: повтор
        if (_repeats > _NEC_SKIP_REPEAT) _repeat = true;
        else _repeats++;
    } else if (time > _NEC_START_MIN && time < _NEC_START_MAX) {    // чтение: начало пакета
        _buffer = _parity = _counter = 0;                           // сбрасываем всё
        _start = true;
    }
}

bool NecDecoder::available() {
    if (_decoded || _repeat) {
        _decoded = _repeat = false;
        return true;
    } return false;
}

bool NecDecoder::isDecoded() {
    if (_decoded) {
        _decoded = false;
        return true;
    } return false;
}

bool NecDecoder::isRepeated() {
    if (_repeat) {
        _repeat = false;
        return true;
    } return false;
}

uint32_t NecDecoder::readPacket() {
    return _packet;
}

uint8_t NecDecoder::readAddress() {
    return ((uint32_t)_packet >> 24);
}

uint8_t NecDecoder::readCommand() {
    return ((uint32_t)_packet >> 8 & 0xFF);
}

uint8_t NecDecoder::readInvCommand() {
    return ((uint32_t)_packet & 0xFF);
}

uint8_t NecDecoder::readInvAddress() {
    return ((uint32_t)_packet >> 16 & 0xFF);
}

bool NecDecoder::addressIsValid() {
    return true;
}

bool NecDecoder::commandIsValid() {
    return true;
}

bool NecDecoder::packetIsValid() {
    return true;
}

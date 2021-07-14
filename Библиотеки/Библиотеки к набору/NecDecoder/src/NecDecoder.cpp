#include "NecDecoder.h"
#define _inRange(a,from,to) (a >= from and a <= to ? true : false) // Возвращает true если 'a' лежит в пределах от from до to

/*
    NEC пакет: (32 бита)
    | [START] | LSB <- [8 бит адрес] <- MSB | LSB <- [8 бит ~адрес] <- MSB | LSB <- [8 бит команда] <- MSB | LSB <- [8 бит ~команда] <- MSB |
*/

void NecDecoder::tick(void) {
  uint32_t period = micros() - _last_edge;                    										// Получаем время последнего импульса
  _last_edge = micros();                              		  										// Обновляем таймер

  if (_inRange(period, _NEC_LOW_BIT - _NEC_TOLERANCE, _NEC_LOW_BIT + _NEC_TOLERANCE)) {  			// (1125 +- _NEC_TOLERANCE) - '0' Бит
    _temp_buffer = _temp_buffer << 1;                                           					// Сдвиг на 1 влево (ничего не прибавляем)
    if (--_remain_counter < 1) {                                                					// Если все 32 бита из предыдущего пакета приняли
#if (NEC_FLOW_CONTROL == true)																		// Если вкючен контроль четности потока
	  if(_parity_control) return;																	// Четность не совпала - пакет битый
#endif
      _packet_buffer = _temp_buffer;                                          						// Перемещаем пакет из составного буфера в основной
	  if(_decode_handler) _decode_handler();														// Если был подключен обработчик - вызвать его
	  else _decoded_flag  = true;                                                  					// Иначе устанавливаем флаг успешного приема пакета      
    }
  } else if (_inRange(period, _NEC_HIGH_BIT - _NEC_TOLERANCE, _NEC_HIGH_BIT + _NEC_TOLERANCE)) {	// (2250 +- _NEC_TOLERANCE) - '1' Бит 
#if (NEC_FLOW_CONTROL == true)																		// Если вкючен контроль четности потока
	_parity_control = !_parity_control;
#endif
    _temp_buffer = _temp_buffer << 1 | 1;                                      				    	// Сдвиг на 1 влево (прибавляем 1)
    if (--_remain_counter < 1) {                                               						// Если все 32 бита из предыдущего пакета приняли
#if (NEC_FLOW_CONTROL == true)																		// Если вкючен контроль четности потока
	  if(_parity_control) return;																	// Четность не совпала - пакет битый
#endif
      _packet_buffer = _temp_buffer;                                          						// Перемещаем пакет из составного буфера в основной
      if(_decode_handler) _decode_handler();														// Если был подключен обработчик - вызвать его;
	  else _decoded_flag  = true;                                                  					// Иначе устанавливаем флаг успешного приема пакета  
    }
  } else if (_inRange(period, _NEC_REPEAT - _NEC_TOLERANCE, _NEC_REPEAT + _NEC_TOLERANCE)) {    	// (11250 +- _NEC_TOLERANCE) - Повтор
    if(_repeat_handler)	_repeat_handler();															// Если был подключен обработчик повтора - вызвать его
    _repeat_flag = true;                                                          					// Иначе устанавливаем флаг повтора
  } else if (_inRange(period, _NEC_START_BIT - _NEC_TOLERANCE, _NEC_START_BIT + _NEC_TOLERANCE)) {  // (13500 +- _NEC_TOLERANCE) - Начало пакета
    _temp_buffer =  0;                                                            					// Очищаем составной буфер
	_parity_control = 0;
    _remain_counter = 32;                                                         					// Сбрасываем счетчик
  }
}

void NecDecoder::attachDecode(void (*handler)(void)) {
  _decode_handler = handler;
}

void NecDecoder::attachRepeat(void (*handler)(void)) {
  _repeat_handler = handler;
}

bool NecDecoder::isDecoded(void) {    // Возвращает true если очередной пакет декодирован
  bool temp = _decoded_flag;     	  // Сохраняем состояние флага
  _decoded_flag = false;       		  // Принудительно очищаем флаг
  return temp;            			  // Возвращаем предыдущее состояннеи флага
}

bool NecDecoder::isRepeated(void) {   // Возвращает true если принята команда повтора
  bool temp = _repeat_flag;     	  // Сохраняем состояние флага
  _repeat_flag = false;       		  // Принудительно очищаем флаг
  return temp;            		      // Возвращаем предыдущее состояннеи флага
}

uint32_t NecDecoder::readPacket(void) {         // Возвращает принятый пакет целиком
  return _packet_buffer;
}

uint8_t NecDecoder::readAddress(void) {         // Прочитать только байт с адресом
  return (uint32_t)(_packet_buffer >> 24);      // Вернуть адрес [Биты 31:24]
}

uint8_t NecDecoder::readInvAddress(void) {        // Прочитать только байт с инвертированным адресом
  return (uint32_t)(_packet_buffer >> 16 & 0xFF); // Вернуть ~адрес [Биты 23:16]
}

uint8_t NecDecoder::readCommand(void) {           // Прочитать только байт с командой
  return (uint32_t)(_packet_buffer >> 8 & 0xFF);  // Вернуть команду [Биты 15:8]
}

uint8_t NecDecoder::readInvCommand(void) {        // Прочитать только байт с инвертированной командой
  return (uint32_t)(_packet_buffer & 0xFF);       // Вернуть ~команду [Биты 7:0]
}

bool NecDecoder::addressIsValid(void) {           // Вернет true если адрес прошел проверку
  return !(readAddress() & readInvAddress());     // Наложение инвертированной битовой маски вернет 0 при совпадении (инвертируем результат)
}

bool NecDecoder::commandIsValid(void) {           // Вернет true если коанда прошла проверку
  return !(readCommand() & readInvCommand());     // Наложение инвертированной битовой маски вернет 0 при совпадении (инвертируем результат)
}

bool NecDecoder::packetIsValid(void) {            // Вернет true если весь пакет прошел проверку
  return addressIsValid() & commandIsValid();     // Вернет true только если обе функции вернут true
}


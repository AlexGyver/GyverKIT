/*
  Простой пример работы с библиотекой NecDecoder.h
  
  Декодируем пакеты с ИК пульта с протоколом NEC вручную (опросом методов isDecoded и isRepeated)
  Для работы необходимо поместить .tick() в обработчик внешнего прерывания по нисходящему (FALLING) фронту.
  
  По протоколу NEC пульт передает 4 байта данных:
  8 бит адреса				-> .readAddress()
  8 бит адреса с инверсией	-> .readInvAddress()
  8 бит команда 			-> .readCommand()
  8 бит команда с инверсией -> .readInvCommand()
  (32 бита пакет целиком) 	-> .readPacket()
  Можно принимать и анализировать пакет целиком (32 бита) или работать с частями отдельно (удобнее и оптимальнее)
  
  Наличие инвертированных копий дает возможность проверить целостность принятого адреса и команды ОТДЕЛЬНО:
  .addressIsValid() - вернет true, если адрес принят в целостности
  .commandIsValid() - вернет true, если команда принята в целостности
  .packetIsValid() - вернет true, если весь пакет в целостности
  
  Как только пакет будет успешно принят метод .isDecoded() вернет true, самостоятельно сбросится в false
  Если будет принята команда повтора, метод .isRepeated() аналогично вернет true, и сам сбросится в false
  
  Команда повтора отправляется пультом при удержании кнопки
  
  Внимание! Библиотека хранит ОДИН (последний принятый) пакет, необходимо обрабатывать его СРАЗУ
*/

//#define NEC_FLOW_CONTROL false // Для отключения контроля четности (по умолчанию включен)
#include "NecDecoder.h"

NecDecoder ir;

void ir_edge() {	// Обработчик внешнего прерывания INT0 по спаду FALLING (ОБЯЗАТЕЛЬНО!)
  ir.tick();  		// Вызываем тикер по спаду
}

void setup() {
  Serial.begin(9600);						// Открываем порт
  attachInterrupt(0, ir_edge, FALLING);		// Внешнее прерывание INT0, по спаду FALLING
}

void loop() {
  if (ir.isDecoded()) {          				 // Если поймали пакет
    if (ir.packetIsValid()) {    				 // И он прошел проверку (еще есть ir.addressIsValid() и ir.commandIsValid())
      Serial.println("\nNEC packet is decoded"); 
      Serial.print("Packet: 0x");
      Serial.println(ir.readPacket(), HEX);      // Пакет целиком
      Serial.print("Address: 0x");
      Serial.println(ir.readAddress(), HEX);     // Только адрес
      Serial.print("~Address: 0x");
      Serial.println(ir.readInvAddress(), HEX);  // Только инвертированный адрес
      Serial.print("Command: 0x");
      Serial.println(ir.readCommand(), HEX);     // Только комманда
      Serial.print("~Command: 0x");
      Serial.println(ir.readInvCommand(), HEX);  // Только инвертированная команда
    } else {
	  Serial.println("\nNEC packet is damaged"); // Иначе - пакет поврежден		
	}
  } else if (ir.isRepeated()) {                  // Если поймали код повтора
    Serial.print("\nRepeat: 0x");                // Выводим последний принятый пакет
    Serial.println(ir.readPacket(), HEX);		
  }
}


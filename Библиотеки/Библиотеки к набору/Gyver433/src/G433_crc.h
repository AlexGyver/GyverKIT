#ifndef G433_crc_h
#define G433_crc_h
#include <Arduino.h>
uint8_t G433_crc8(uint8_t *buffer, uint8_t size);       // ручной CRC8
uint8_t G433_crc_xor(uint8_t *buffer, uint8_t size);    // ручной CRC XOR
void G433_crc8_byte(uint8_t &crc, uint8_t data);        // crc8 один байт
#endif
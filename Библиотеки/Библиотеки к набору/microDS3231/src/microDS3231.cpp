#include "microDS3231.h"

//static const uint8_t _ds_daysInMonth[] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
static uint8_t _ds_DIM(uint8_t i) {
    return (i < 7) ? ((i == 1) ? 28 : ((i & 1) ? 30 : 31)) : ((i & 1) ? 31 : 30);
}

static uint16_t getWeekDay(uint16_t y, uint8_t m, uint8_t d) {
    if (y >= 2000)
    y -= 2000;
    uint16_t days = d;
    for (uint8_t i = 1; i < m; ++i)
    //days += pgm_read_byte(_ds_daysInMonth + i - 1);
    days += _ds_DIM(i - 1);
    if (m > 2 && y % 4 == 0)
    ++days;
    return (days + 365 * y + (y + 3) / 4 + 4) % 7 + 1;
}

// поiхали
MicroDS3231::MicroDS3231(uint8_t addr) : _addr(addr) {
    Wire.begin();
}

bool MicroDS3231::begin(void){
	Wire.begin();                       // Инит шины
	Wire.beginTransmission(_addr);      // Зовем DS3231 по адресу
    return (!Wire.endTransmission());   // если никто не откликнулся - возвращаем false
}

void MicroDS3231::setTime(int8_t seconds, int8_t minutes, int8_t hours, int8_t date, int8_t month, int16_t year) {
    // защиты от дурака
    month = constrain(month, 1, 12);
    //date = constrain(date, 0, pgm_read_byte(_ds_daysInMonth + month - 1));
    date = constrain(date, 0, _ds_DIM(month - 1));
    seconds = constrain(seconds, 0, 59);
    minutes = constrain(minutes, 0, 59);
    hours = constrain(hours, 0, 23);
    
    // отправляем
    uint8_t day = getWeekDay(year, month, date);
    year -= 2000;
    Wire.beginTransmission(_addr);
    Wire.write(0x00);
    Wire.write(encodeRegister(seconds));
    Wire.write(encodeRegister(minutes));
    if (hours > 19) Wire.write((0x2 << 4) | (hours % 20));
    else if (hours > 9) Wire.write((0x1 << 4) | (hours % 10));
    else Wire.write(hours);	
    Wire.write(day);
    Wire.write(encodeRegister(date));
    Wire.write(encodeRegister(month));
    Wire.write(encodeRegister(year));
    Wire.endTransmission();
}

void MicroDS3231::setHMSDMY(int8_t hours, int8_t minutes, int8_t seconds, int8_t date, int8_t month, int16_t year) {
    setTime(seconds, minutes, hours, date, month, year);
}

void MicroDS3231::setTime(DateTime time) {
    setTime(time.second, time.minute, time.hour, time.date, time.month, time.year);
}

static int charToDec(const char* p) {
    return (10 * (*p - '0') + (*++p - '0'));
}

void MicroDS3231::setTime(const __FlashStringHelper* stamp) {
    char buff[25];   
    memcpy_P(buff, stamp, 25);
    
    // Wed Jul 14 22:00:24 2021
    //     4   8  11 14 17   22
    // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
    int h, m, s, d, mo, y;
    h = charToDec(buff + 11);
    m = charToDec(buff + 14);
    s = charToDec(buff + 17);
    d = charToDec(buff + 8);  
    switch (buff[4]) {
    case 'J': mo = (buff[5] == 'a') ? 1 : (mo = (buff[6] == 'n') ? 6 : 7); break;
    case 'F': mo = 2; break;
    case 'A': mo = (buff[6] == 'r') ? 4 : 8; break;
    case 'M': mo = (buff[6] == 'r') ? 3 : 5; break;
    case 'S': mo = 9; break;
    case 'O': mo = 10; break;
    case 'N': mo = 11; break;
    case 'D': mo = 12; break;
    }
    y = 2000 + charToDec(buff + 22);
    setTime(s, m, h, d, mo, y);
}

DateTime MicroDS3231::getTime() {
    DateTime now;
    Wire.beginTransmission(_addr);
    Wire.write(0x0);
    if (Wire.endTransmission() != 0) return now;
    Wire.requestFrom(_addr, (uint8_t)7);
    now.second = unpackRegister(Wire.read());
    now.minute = unpackRegister(Wire.read());
    now.hour = unpackHours(Wire.read());
    now.day = Wire.read();
    now.date = unpackRegister(Wire.read());
    now.month = unpackRegister(Wire.read());
    now.year = unpackRegister(Wire.read()) + 2000;
    return now;
}
String MicroDS3231::getTimeString() {
    DateTime now = getTime();
    String str = "";
    if (now.hour < 10) str += '0';
    str += now.hour;
    str += ':';
    if (now.minute < 10) str += '0';
    str += now.minute;
    str += ':';
    if (now.second < 10) str += '0';
    str += now.second;
    return str;
}
String MicroDS3231::getDateString() {
    DateTime now = getTime();
    String str = "";
    if (now.date < 10) str += '0';
    str += now.date;
    str += '.';
    if (now.month < 10) str += '0';
    str += now.month;
    str += '.';
    str += now.year;    
    return str;
}
void MicroDS3231::getTimeChar(char* array) {
    DateTime now = getTime();
    array[0] = now.hour / 10 + '0';
    array[1] = now.hour % 10 + '0';
    array[2] = ':';
    array[3] = now.minute / 10 + '0';
    array[4] = now.minute % 10 + '0';
    array[5] = ':';
    array[6] = now.second / 10 + '0';
    array[7] = now.second % 10 + '0';
    array[8] = '\0';
}
void MicroDS3231::getDateChar(char* array) {
    DateTime now = getTime();        
    array[0] = now.date / 10 + '0';
    array[1] = now.date % 10 + '0';
    array[2] = '.';
    array[3] = now.month / 10 + '0';
    array[4] = now.month % 10 + '0';
    array[5] = '.';
    itoa(now.year, array + 6, DEC);
    array[10] = '\0';
}
bool MicroDS3231::lostPower(void) { // возвращает true, если 1 января 2000
    if (getYear() == 2000 && getMonth() == 1 && getDate() == 1) return true;
    else return false;
}

uint8_t MicroDS3231::getSeconds(void) {
    return (unpackRegister(readRegister(0x00)));
}

uint8_t MicroDS3231::getMinutes(void) {
    return (unpackRegister(readRegister(0x01)));
}

uint8_t MicroDS3231::getHours(void) {
    return (unpackHours(readRegister(0x02)));
}

uint8_t MicroDS3231::getDay(void) {
    return readRegister(0x03);
}

uint8_t MicroDS3231::getDate(void) {
    return (unpackRegister(readRegister(0x04)));
}

uint8_t MicroDS3231::getMonth(void) {
    return (unpackRegister(readRegister(0x05)));
}

uint16_t MicroDS3231::getYear(void) {
    return (unpackRegister(readRegister(0x06)) + 2000);
}

// сервис
uint8_t MicroDS3231::readRegister(uint8_t addr) {
    Wire.beginTransmission(_addr);
    Wire.write(addr);
    if (Wire.endTransmission() != 0) return 0;
    Wire.requestFrom(_addr, (uint8_t)1);
    uint8_t data = Wire.read();
    return data;
}

uint8_t MicroDS3231::unpackRegister(uint8_t data) {
    return ((data >> 4) * 10 + (data & 0xF));
}

uint8_t MicroDS3231::encodeRegister(int8_t data) {
    return (((data / 10) << 4) | (data % 10));
}

uint8_t MicroDS3231::unpackHours(uint8_t data) {
    if (data & 0x20) return ((data & 0xF) + 20);
    else if (data & 0x10) return ((data & 0xF) + 10);
    else return (data & 0xF);
}

float MicroDS3231::getTemperatureFloat(void) {
    return (getTemperatureRaw() * 0.25f);
}

int MicroDS3231::getTemperature(void) {
    return (getTemperatureRaw() >> 2);
}

int MicroDS3231::getTemperatureRaw(void) {
    Wire.beginTransmission(_addr);
    Wire.write(0x11);
    Wire.endTransmission();
    Wire.requestFrom(_addr, (uint8_t)2);
    return (Wire.read() << 2 | Wire.read() >> 6);
}
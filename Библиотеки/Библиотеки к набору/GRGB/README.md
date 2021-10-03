![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# GRGB
Библиотека для управления RGB светодиодами и лентами для Arduino. Облегчённая версия библиотеки GyverRGB
- Поддержка драйверов с общим анодом и общим катодом
- Настройка яркости
- Гамма-коррекция яркости (квадратный CRT)    
- Библиотека может не привязываться к пинам и просто генерировать значения 8 бит
- Быстрые оптимизированные целочисленные вычисления (не везде)
- Плавный переход между любыми цветами (не блокирует выполнение кода)
- Установка цвета разными способами:
    - RGB
    - HSV
    - Быстрый HSV
    - Цветовое колесо (1530 значений)
    - Цветовое колесо (255 значений)
    - Теплота (1000-40000К)
    - HEX цвета 24 бита
    - HEX цвета 16 бит
    - 17 предустановленных цветов

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **GRGB** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GRGB/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
// указать тип драйвера COMMON_CATHODE/COMMON_ANODE
// и пины в порядке R,G,B
GRGB led(COMMON_CATHODE, pinR, pinG, pinB);

// режим 10 бит ШИМ (для esp8266)
GRGB led(COMMON_CATHODE, pinR, pinG, pinB, GRGB_10BIT);

// виртуальный драйвер - пины не указываются
GRGB led(COMMON_CATHODE);
```

<a id="usage"></a>
## Использование
```cpp
void enable();                                                  // вкл 
void disable();                                                 // выкл
void setPower(bool power);                                      // вкл/выкл
void setRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t br = 255); // установить цвета r, g, b: 0-255 + опционально яркость
void setHSVfast(uint8_t h, uint8_t s, uint8_t v);               // быстрый HSV
void setHSV(uint8_t h, uint8_t s, uint8_t v);                   // обычный HSV
void setWheel(int color, uint8_t br = 255);                     // цветовое колесо 0-1530  + опционально яркость
void setWheel8(uint8_t color, uint8_t br = 255);                // цветовое колесо 0-255  + опционально яркость
void setKelvin(int kelvin, uint8_t br = 255);                   // цветовая температура 1000-40000К  + опционально яркость
void setKelvinFast(int kelvin, uint8_t br = 255);               // быстрое вычисление. Цветовая температура 1000-10000К  + опционально яркость
void setHEX(uint32_t color, uint8_t br = 255);                  // HEX цвет 24 бита  + опционально яркость
void setHEX16(uint32_t color, uint8_t br = 255);                // HEX цвет 16 бит  + опционально яркость
void setColor(uint32_t color, uint8_t br = 255);                // цвет из предустановленных RGBCOLORS  + опционально яркость
void setBrightness(uint8_t bri);                                // общая яркость 0-255
void setCRT(bool crt);                                          // вкл/выкл гамма-коррекцию яркости
bool tick();                                                    // тикер плавной смены цвета, вызывать как можно чаще если включен
void fadeMode(bool fade);                                       // включение/выключение режима плавной смены цвета (по умолч. выкл)
void setFadePeriod(uint32_t fadeTime);                          // установить продолжительность смены цвета
void attach(void (*handler)());                                 // подключить коллбэк
void detach();                                                  // отключить коллбэк
uint8_t R, G, B;                                                // 8 бит сигналы для "виртуального" драйвера, обновляются после установки цвета
```

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!
```cpp
#include <GRGB.h>
// указать тип драйвера COMMON_CATHODE/COMMON_ANODE
// и пины в порядке R,G,B
GRGB led(COMMON_CATHODE, 9, 10, 11);

void setup() {
  // установить ОБЩУЮ яркость
  led.setBrightness(255);

  // включить гамма-коррекцию (по умолч. выключено)
  led.setCRT(true);
  //led.setCRT(false);  // выключить

  // установить цвет RGB [0-255]
  led.setRGB(255, 0, 0);
  //led.setRGB(255, 0, 0, 120); // + "локальная" яркость [0-255]
  delay(1000);

  // установить цвет HSV (цвет, насыщенность, яркость) [0-255]
  led.setHSV(120, 200, 250);
  delay(1000);

  // установить цвет HSV быстрый алгоритм (цвет, насыщенность, яркость) [0-255]
  led.setHSVfast(120, 200, 250);
  delay(1000);

  // установить цвет из цветового колеса [0-1530]
  led.setWheel(800);
  //led.setWheel(800, 120); // + "локальная" яркость [0-255]
  delay(1000);

  // установить цвет из цветового колеса [0-255]
  led.setWheel8(120);
  //led.setWheel8(120, 255); // + "локальная" яркость [0-255]
  delay(1000);

  // установить цветовую температуру [1000-40000] Кельвинов
  led.setKelvin(5500);
  //led.setKelvin(5500, 255); // + "локальная" яркость [0-255]
  delay(1000);

  // установить HEX цвет (24 бита)
  led.setHEX(0x00FF00);
  //led.setHEX(0x00FF00, 120); // + "локальная" яркость [0-255]
  delay(1000);

  // установить HEX цвет (16 бит)
  led.setHEX16(0xF81F);
  //led.setHEX16(0xF81F, 120); // + "локальная" яркость [0-255]
  delay(1000);

  // установить предустановленный цвет (16 бит)
  led.setColor(GAqua);  // список цветов в GRGB.h
  //led.setColor(GAqua, 120); // + "локальная" яркость [0-255]
  delay(1000);
}

void loop() {
  // плавно мигаем яркостью
  static uint32_t tmr;
  if (millis() - tmr >= 5) {
    tmr = millis();
    static int8_t dir = 1;
    static int val = 0;
    val += dir;
    if (val == 255 || val == 0) dir = -dir;   // разворачиваем
    led.setBrightness(val);
  }
}
```

<a id="versions"></a>
## Версии
- v1.0
- v1.1 - добавлен setKelvinFast
- v1.2 - добавил псевдо 10 бит
- v1.2.1 - исправлен баг
- v1.3 - исправлена локальная яркость
- v1.3.1 - исправлена инверсия для 10 бит
- v1.4 - добавил enable(), disable() и setPower(bool)
- v1.4.1 - совместимость Digispark

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!
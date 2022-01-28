![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# GyverMotor
Библиотека для удобного управления коллекторными моторами через драйвер
- Контроль скорости и направления вращения
- Работа с 10 битным ШИМом
- Плавный пуск и изменение скорости
- Порог минимальной скважности
- Deadtime
- Поддержка 4х типов драйверов

### Совместимость
Совместима со всеми Arduino платформами (используются Arduino-функции)

### Документация
К библиотеке есть [расширенная документация](https://alexgyver.ru/GyverMotor/)

## Содержание
- [Установка](#install)
- [Инициализация](#init)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- Библиотеку можно найти по названию **GyverMotor** и установить через менеджер библиотек в:
    - Arduino IDE
    - Arduino IDE v2
    - PlatformIO
- [Скачать библиотеку](https://github.com/GyverLibs/GyverMotor/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="init"></a>
## Инициализация
```cpp
// варианты инициализации в зависимости от типа драйвера:
GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW / HIGH) );
GMotor motor(DRIVER2WIRE_NO_INVERT, dig_pin, PWM_pin, (LOW / HIGH) );
GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) );
GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) );
/*
  DRIVER2WIRE - двухпроводной драйвер (направление + ШИМ)
  DRIVER2WIRE_NO_INVERT - двухпроводной драйвер, в котором при смене направления не нужна инверсия ШИМ
  DRIVER3WIRE - трёхпроводной драйвер (два пина направления + ШИМ)
  RELAY2WIRE - реле в качестве драйвера (два пина направления)

  dig_pin, dig_pin_A, dig_pin_B - любой цифровой пин МК
  PWM_pin - любой ШИМ пин МК
  LOW / HIGH - уровень драйвера. Если при увеличении скорости мотор наоборот тормозит - смени уровень
*/
```

<a id="usage"></a>
## Использование
```cpp
GMotor(GM_driverType type, int8_t param1 = _GM_NC, int8_t param2 = _GM_NC, int8_t param3 = _GM_NC, int8_t param4 = _GM_NC);
// три варианта создания объекта в зависимости от драйвера:
// GMotor motor(DRIVER2WIRE, dig_pin, PWM_pin, (LOW/HIGH) )
// GMotor motor(DRIVER3WIRE, dig_pin_A, dig_pin_B, PWM_pin, (LOW/HIGH) )
// GMotor motor(RELAY2WIRE, dig_pin_A, dig_pin_B, (LOW/HIGH) )

// установка скорости -255..255 (8 бит) и -1023..1023 (10 бит)
void setSpeed(int16_t duty);

// сменить режим работы мотора:	
// FORWARD - вперёд
// BACKWARD - назад
// STOP - остановить
// BRAKE - активный тормоз
// AUTO - подчиняется setSpeed (-255.. 255)
void setMode(GM_workMode mode);

// направление вращения	
// NORM - обычное
// REVERSE - обратное
void setDirection(bool direction);

// установить минимальную скважность (при которой мотор начинает крутиться)
void setMinDuty(int duty);

// установить разрешение ШИМ в битах
void setResolution(byte bit);

// установить deadtime (в микросекундах). По умолч 0
void setDeadtime(uint16_t deadtime);	

// установить уровень драйвера (по умолч. HIGH)
void setLevel(int8_t level);			

// плавное изменение к указанной скорости (к значению ШИМ)
void smoothTick(int16_t duty);

// скорость изменения скорости
void setSmoothSpeed(uint8_t speed);	

// возвращает -1 при вращении BACKWARD, 1 при FORWARD и 0 при остановке и торможении
int getState();

// внутренняя переменная скважности для отладки
int16_t _duty = 0;

// свовместимость со старыми версиями
// установить выход в 8 бит
void set8bitMode();		

// установить выход в 10 бит
void set10bitMode();
```

### Логика работы
В setMinDuty() можно установить минимальную скорость (0..255), при которой мотор начинает вращение. 
Дальнейшие настройки скорости будут автоматически масштабироваться с учётом минимальной.  
setDirection() задаёт глобальное направление мотора, которое автоматически влияет на все функции скорости.

#### Обычный режим
Запускается setMode(FORWARD) для движения вперёд, setMode(BACKWARD) - назад. 
Скорость устанавливается в setSpeed() либо run(FORWARD/BACKWARD, скорость). Остановить можно setMode(STOP).

#### Авто режим
Запускается setMode(AUTO), скорость задаётся в setSpeed(), поддерживаются отрицательные значения для вращения в другую сторону. 
Остановить можно setMode(STOP).

#### Плавный режим
Для запуска нужно установить setMode(AUTO). В плавном режиме нужно почаще вызывать smoothTick с указанием целевой скорости. При значении 0 мотор сам плавно остановится. 
Для резкой остановки можно использовать setMode(STOP).

<a id="example"></a>
## Пример
Остальные примеры смотри в **examples**!
```cpp
/*
	Пример управления мотором при помощи драйвера полного моста и потенциометра
*/
#include "GyverMotor.h"
GMotor motor(DRIVER2WIRE, 2, 3, HIGH);

void setup() {
  // установка программного deadtime на переключение направления, микросекунды
  // по умолчанию стоит 0: deadtime отключен
  // motor.setDeadtime(200);

  // ГЛОБАЛЬНАЯ смена направления вращения мотора
  // например чтобы FORWARD совпадал с направлением движения "вперёд" у машинки  
  motor.setDirection(REVERSE);
  motor.setDirection(NORMAL);   // умолч.

  // смена режима работы мотора
  motor.setMode(FORWARD);   // вперёд
  motor.setMode(BACKWARD);  // назад
  motor.setMode(BRAKE);  	// активный тормоз
  motor.setMode(STOP);      // стоп, холостой (мотор отключен)
  motor.setMode(AUTO);      // авторежим

  // смена уровня драйвера (аналогично при инициализации)
  // Если при увеличении скорости мотор наоборот тормозит - смени уровень
  motor.setLevel(LOW);
  motor.setLevel(HIGH);     // по умолч.
  
  // для работы в 10 бит необходимо также настроить ШИМ на 10 бит!!!
  // читай тут https://alexgyver.ru/lessons/pwm-overclock/
  // motor.setResolution(10);

  // минимальный сигнал (по модулю), который будет подан на мотор
  // Избавляет от ситуаций, когда мотор покоится и "пищит"
  motor.setMinDuty(150);

  // ключ на старт!
  motor.setMode(FORWARD);
}

void loop() {
  // потенциометр на А0
  // преобразуем значение в -255.. 255
  int val = 255 - analogRead(0) / 2;

  // установка скорости:
  // * (0..255) при ручном выборе направления (setMode: FORWARD/BACKWARD)
  // * (-255..255) при автоматическом (поставить setMode(FORWARD))
  // * (0..1023) в режиме 10 бит при ручном выборе направления (setMode: FORWARD/BACKWARD)
  // * (-1023..1023) в режиме 10 бит при автоматическом (поставить setMode(FORWARD))    
  
  motor.setSpeed(val);
  // в данном случае мотор будет остановлен в среднем положении рукоятки
  // и разгоняться в противоположные скорости в крайних её положениях
  
  delay(10);  // задержка просто для "стабильности"
}

```

<a id="versions"></a>
## Версии
- v1.1 - убраны дефайны
- v1.2 - возвращены дефайны
- v2.0:
    - Программный deadtime
    - Отрицательные скорости
    - Поддержка двух типов драйверов и реле
    - Плавный пуск и изменение скорости
- v2.1: небольшие фиксы и добавления
- v2.2: оптимизация
- v2.3: добавлена поддержка esp (исправлены ошибки)
- v2.4: совместимость с другими библами
- v2.5: добавлен тип DRIVER2WIRE_NO_INVERT
- v3.0: переделана логика minDuty, добавлен режим для ШИМ любой битности
- v3.1: мелкие исправления
- v3.2: улучшена стабильность плавного режима
- v3.2.1: вернул run() в public

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!

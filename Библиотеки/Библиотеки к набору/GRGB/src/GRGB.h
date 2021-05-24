/*
    Библиотека для управления RGB светодиодами и лентами. Облегчённая версия библиотеки GyverRGB
    Документация: 
    GitHub: https://github.com/GyverLibs/GRGB
    Возможности:
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
    
    AlexGyver, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
*/

#ifndef GRGB_h
#define GRGB_h

#define COMMON_CATHODE 0
#define COMMON_ANODE 1

enum RGBCOLORS {
    GWhite =	0xFFFF,		// белый
    GSilver =	0xC618,		// серебро
    GGray =		0x8410,		// серый
    GBlack =	0x0,		// чёрный	
    GRed =		0xF800,		// красный
    GMaroon =	0x8000,		// бордовый
    GOrange =	0xF980,		// оранжевый
    GYellow =	0xFC00,		// жёлтый
    GOlive =	0x8400,		// олива
    GLime =		0x7E0,		// лайм
    GGreen =	0x400,		// зелёный
    GAqua =		0x7FF,		// аква
    GTeal =		0x410,		// цвет головы утки чирка
    GBlue =		0x1F,		// голубой
    GNavy =		0x10,		// тёмно-синий
    GMagenta =	0xF81F,		// розовый
    GPurple =	0x8010,		// пурпурный
};

class GRGB {
public:
    // пины в порядке RGB + опционально тип (по умолч. общий катод)
    GRGB(const uint8_t dir = COMMON_CATHODE, const uint8_t pinR = 0, const uint8_t pinG = 0, const uint8_t pinB = 0) :
    _pinR(pinR), _pinG(pinG), _pinB(pinB), _dir(dir) {
        // если пины указаны (не равны)
        if (!(_pinR == _pinG && _pinR == _pinB)) {
            pinMode(pinR, OUTPUT);
            pinMode(pinG, OUTPUT);
            pinMode(pinB, OUTPUT);
        }
    }
    
    // установить цвета r, g, b: 0-255
    void setRGB(uint8_t r, uint8_t g, uint8_t b, uint8_t br = 255) {
        _r = r;
        _g = g;
        _b = b;
        if (br != 255) {
            _fade8(_r);
            _fade8(_g);
            _fade8(_b);
        }
        show();
    }
    
    // быстрый HSV
    void setHSVfast(uint8_t h, uint8_t s, uint8_t v) {        
        uint8_t value = ((24 * h / 17) / 60) % 6;
        uint8_t vmin = (long)v - v * s / 255;
        uint8_t a = (long)v * s / 255 * (h * 24 / 17 % 60) / 60;
        uint8_t vinc = vmin + a;
        uint8_t vdec = v - a;
        switch (value) {
        case 0: _r = v; _g = vinc; _b = vmin; break;
        case 1: _r = vdec; _g = v; _b = vmin; break;
        case 2: _r = vmin; _g = v; _b = vinc; break;
        case 3: _r = vmin; _g = vdec; _b = v; break;
        case 4: _r = vinc; _g = vmin; _b = v; break;
        case 5: _r = v; _g = vmin; _b = vdec; break;
        }
        show();
    }
    
    // обычный HSV
    void setHSV(uint8_t h, uint8_t s, uint8_t v) {        
        float r, g, b;
        
        float H = h / 255.0;
        float S = s / 255.0;
        float V = v / 255.0;
        
        int i = int(H * 6);
        float f = H * 6 - i;
        float p = V * (1 - S);
        float q = V * (1 - f * S);
        float t = V * (1 - (1 - f) * S);
        
        switch (i % 6) {
        case 0: r = V, g = t, b = p; break;
        case 1: r = q, g = V, b = p; break;
        case 2: r = p, g = V, b = t; break;
        case 3: r = p, g = q, b = V; break;
        case 4: r = t, g = p, b = V; break;
        case 5: r = V, g = p, b = q; break;	
        }
        _r = r * 255.0;
        _g = g * 255.0;
        _b = b * 255.0;
        show();
    }
    
    // цветовое колесо 0-1530
    void setWheel(int color, uint8_t br = 255) {
        if (color <= 255) {           			  // красный макс, зелёный растёт
            _r = 255;
            _g = color;
            _b = 0;
        }
        else if (color > 255 && color <= 510) {   // зелёный макс, падает красный
            _r = 510 - color;
            _g = 255;
            _b = 0;
        }
        else if (color > 510 && color <= 765) {   // зелёный макс, растёт синий
            _r = 0;
            _g = 255;
            _b = color - 510;
        }
        else if (color > 765 && color <= 1020) {  // синий макс, падает зелёный
            _r = 0;
            _g = 1020 - color;
            _b = 255;
        }
        else if (color > 1020 && color <= 1275) { // синий макс, растёт красный
            _r = color - 1020;
            _g = 0;
            _b = 255;
        }
        else if (color > 1275 && color <= 1530) { // красный макс, падает синий
            _r = 255;
            _g = 0;
            _b = 1530 - color;
        }
        if (br != 255) {
            _fade8(_r);
            _fade8(_g);
            _fade8(_b);
        }
        show();
    }
    
    // цветовое колесо 0-255
    void setWheel8(uint8_t color, uint8_t br = 255) {
        uint8_t shift;
        if (color > 170) {
            shift = (color - 170) * 3;
            _r = shift;
            _g = 0;
            _b = 255 - shift;
        } else if (color > 85) {
            shift = (color - 85) * 3;
            _r = 0;
            _g = 255 - shift;
            _b = shift;
        } else {
            shift = color * 3;
            _r = 255 - shift;
            _g = shift;
            _b = 0;
        }
        if (br != 255) {
            _fade8(_r);
            _fade8(_g);
            _fade8(_b);
        }
        show();
    }
    
    // цветовая температура 1000-40000К
    void setKelvin(int kelvin, uint8_t br = 255) {
        float tmpKelvin, tmpCalc;
        kelvin = constrain(kelvin, 1000, 40000);
        tmpKelvin = kelvin / 100;
        
        // red
        if (tmpKelvin <= 66) _r = 255;
        else {
            tmpCalc = tmpKelvin - 60;
            tmpCalc = (float)pow(tmpCalc, -0.1332047592);
            tmpCalc *= (float)329.698727446;
            tmpCalc = constrain(tmpCalc, 0, 255);
            _r = tmpCalc;
        }
        
        // green
        if (tmpKelvin <= 66) {
            tmpCalc = tmpKelvin;
            tmpCalc = (float)99.4708025861 * log(tmpCalc) - 161.1195681661;
            tmpCalc = constrain(tmpCalc, 0, 255);
            _g = tmpCalc;
        } else {
            tmpCalc = tmpKelvin - 60;
            tmpCalc = (float)pow(tmpCalc, -0.0755148492);
            tmpCalc *= (float)288.1221695283;
            tmpCalc = constrain(tmpCalc, 0, 255);
            _g = tmpCalc;
        }
        
        // blue
        if (tmpKelvin >= 66) _b = 255;
        else if (tmpKelvin <= 19) _b = 0;
        else {
            tmpCalc = tmpKelvin - 10;
            tmpCalc = (float)138.5177312231 * log(tmpCalc) - 305.0447927307;
            tmpCalc = constrain(tmpCalc, 0, 255);
            _b = tmpCalc;
        }
        if (br != 255) {
            _fade8(_r);
            _fade8(_g);
            _fade8(_b);
        }
        show();
    }
    
    // HEX цвет 24 бита
    void setHEX(uint32_t color, uint8_t br = 255) {
        // буфер цветов
        _r = ((uint32_t)color >> 16) & 0xFF;
        _g = ((uint32_t)color >> 8) & 0xFF;
        _b = (uint32_t)color & 0xFF;
        if (br != 255) {
            _fade8(_r);
            _fade8(_g);
            _fade8(_b);
        }
        show();
    }
    
    // HEX цвет 16 бит
    void setHEX16(uint32_t color) {
        setHEX( ((color & 0xF800) << 8) | ((color & 0x7E0) << 5) | ((color & 0x1F) << 3) ); // rgb16->rgb24
    }
    
    // цвет из предустановленных RGBCOLORS
    void setColor(uint32_t color) {
        setHEX16(color);
    }
    
    // яркость 0-255
    void setBrightness(uint8_t bri) {
        _bri = bri;
        show();
    }
    
    // вкл/выкл гамма-коррекцию яркости
    void setCRT(bool crt) {
        _crt = crt;
    }
    
    // тикер плавной смены цвета, вызывать как можно чаще если включен
    bool tick() {
        if (_fade && _fadeFlag && millis() - _tmr >= _fadeDt) {
            _tmr = millis();            
            _rf += _stepR;          // меняем цвет
            _gf += _stepG;
            _bf += _stepB;
            _steps--;
            if (!_steps) {          // изменение закончено
                _fadeFlag = false;  // флаг вниз
                _rf = _r << 7;      // применить финальный цвет
                _gf = _g << 7;
                _bf = _b << 7;
            }
            show(true);     // показать цвета фейдера
            /*Serial.print(_rf>>7) ;Serial.print(',');
            Serial.print(_gf>>7); Serial.print(',');
            Serial.println(_bf>>7);*/
        }
        return (_fade && _fadeFlag);
    }
    
    // включение/выключение режима плавной смены цвета (по умолч. выкл)
    void fadeMode(bool fade) {
        _fade = fade;
    }
    
    // установить продолжительность смены цвета
    void setFadePeriod(uint32_t fadeTime) {
        _fadeTime = fadeTime;
    }
    
    // подключить коллбэк
    void attach(void (*handler)()) {
        _handler = *handler;
    }
    
    // отключить коллбэк
    void detach() {
        _handler = NULL;
    }
    
    // 8 бит сигналы для "виртуального" драйвера, обновляются после установки цвета
    uint8_t R, G, B;    
    
private:    
    // отобразить
    void show(bool fader = 0) {
        uint8_t r = _r, g = _g, b = _b;
        if (_fade) {                                    // фейд включен
            if (!fader) {                               // нужно установить новый цвет
                // изменения по каналам в 8 бит
                int16_t deltaR = _r - (_rf >> 7);
                int16_t deltaG = _g - (_gf >> 7);
                int16_t deltaB = _b - (_bf >> 7);
                
                // макс изменение в 8 бит
                _steps = 0;
                _steps = max(abs(deltaR), _steps);
                _steps = max(abs(deltaG), _steps);
                _steps = max(abs(deltaB), _steps);
                
                // если есть что менять
                if (_steps > 0) {
                    _fadeDt = _fadeTime / _steps;               // мс между шагами                
                    _fadeDt = max(_fadeDt, 20);                 // не менее 20 мс
                    _steps = _fadeTime / _fadeDt;               // общее кол-во шагов
                    _stepR = ((long)(_r << 7) - _rf) / _steps;  // шаг R
                    _stepG = ((long)(_g << 7) - _gf) / _steps;  // шаг G
                    _stepB = ((long)(_b << 7) - _bf) / _steps;  // шаг B
                    _fadeFlag = true;                           // флаг на процесс
                    _tmr = millis();                            // сбросить таймер
                }
            }
            r = _rf >> 7;
            g = _gf >> 7;
            b = _bf >> 7;            
        } else {    // фейд выключен
            _rf = r << 7;
            _gf = g << 7;
            _bf = b << 7;
        }
        
        // глобальная яркость
        _fade8(r);
        _fade8(g);
        _fade8(b);
        
        // CRT
        if (_crt) {
            _crtSq(r);
            _crtSq(g);
            _crtSq(b);
        }
        
        // направление
        if (_dir) {
            r = 255 - r;
            g = 255 - g;
            b = 255 - b;
        }
        
        // для фронтенда
        R = r;
        G = g;
        B = b;
        if (*_handler) _handler();
        
        // если пины указаны, выводим
        if (!(_pinR == _pinG && _pinR == _pinB)) {
            analogWrite(_pinR, r);
            analogWrite(_pinG, g);
            analogWrite(_pinB, b);
        }
    }
    void _fade8(uint8_t& x) { x = ((uint16_t)x * (_bri + 1)) >> 8; }
    void _crtSq(uint8_t& x) { x = ((uint16_t)(x) * (x) + 255) >> 8; }
    
    const uint8_t _pinR, _pinG, _pinB, _dir;
    uint8_t _bri = 255;
    bool _crt = false, _fade = false;
    uint8_t _r, _g, _b;
    
    uint16_t _rf = 0, _gf = 0, _bf = 0;
    uint32_t _tmr = 0, _fadeTime = 2000;
    uint16_t _fadeDt;
    int16_t _stepR = 0, _stepG = 0, _stepB = 0;
    int16_t _steps;
    bool _fadeFlag = false;
    void (*_handler)() = NULL;
};
#endif
// демо возможностей библиотеки
#include <microDS3231.h>
MicroDS3231 rtc;

void setup() {
  Serial.begin(9600);
  
  // проверка наличия модуля на линии i2c
  // вызов rtc.begin() не обязателен для работы
  if (!rtc.begin()) {
    Serial.println("DS3231 not found");
    for(;;);
  }
  
  // ======== УСТАНОВКА ВРЕМЕНИ АВТОМАТИЧЕСКИ ========
  // rtc.setTime(COMPILE_TIME);     // установить время == времени компиляции
  
  // визуально громоздкий, но более "лёгкий" с точки зрения памяти способ установить время компиляции
  rtc.setTime(BUILD_SEC, BUILD_MIN, BUILD_HOUR, BUILD_DAY, BUILD_MONTH, BUILD_YEAR);
    
  if (rtc.lostPower()) {            // выполнится при сбросе батарейки
    Serial.println("lost power!");
    // тут можно однократно установить время == времени компиляции
  }
  
  // ======== УСТАНОВКА ВРЕМЕНИ ВРУЧНУЮ ========    
  // установить время вручную можно двумя способами (подставить реальные числа)
  //rtc.setTime(SEC, MIN, HOUR, DAY, MONTH, YEAR);
  //rtc.setHMSDMY(HOUR, MIN, SEC, DAY, MONTH, YEAR);
  
  // также можно установить время через DateTime
  /*
  DateTime now;
  now.second = 0;
  now.minute = 10;
  now.hour = 50;
  now.date = 2;
  now.month = 9;
  now.year = 2021;
  
  rtc.setTime(now);  // загружаем в RTC
  */
}

void loop() {
  // получение и вывод каждой компоненты
  Serial.print(rtc.getHours());
  Serial.print(":");
  Serial.print(rtc.getMinutes());
  Serial.print(":");
  Serial.print(rtc.getSeconds());
  Serial.print(" ");
  Serial.print(rtc.getDay());
  Serial.print(" ");
  Serial.print(rtc.getDate());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.println(rtc.getYear());
  
  /*
  // можно через DateTime (более оптимально):
  DateTime now = rtc.getTime();
  Serial.print(now.hour);
  Serial.print(":");
  Serial.print(now.minute);
  Serial.print(":");
  Serial.print(now.second);
  Serial.print(" ");
  Serial.print(now.day);
  Serial.print(" ");
  Serial.print(now.date);
  Serial.print("/");
  Serial.print(now.month);
  Serial.print("/");
  Serial.println(now.year);
  */
  
  // вывод температуры чипа
  Serial.println(rtc.getTemperatureFloat());  
  //Serial.println(rtc.getTemperature());
  
  // вывод времени готовой строкой String
  Serial.println(rtc.getTimeString());
  
  // вывод даты готовой строкой String
  Serial.println(rtc.getDateString());

  // вывод времени через char array
  char time[8];
  rtc.getTimeChar(time);
  Serial.println(time);
  
  // вывод даты через char array
  char date[10];
  rtc.getDateChar(date);
  Serial.println(date);
  
  Serial.println();
  delay(500);
}

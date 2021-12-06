// чтение, блокирующий вариант

#include <GyverHTU21D.h>
GyverHTU21D htu;

void setup() {
  Serial.begin(9600);
  if (!htu.begin()) Serial.println(F("HTU21D error"));
}

void loop() {
  // время выполнения зависит от разрешения, см. доку
  float temp = htu.getTemperatureWait();
  float hum = htu.getHumidityWait();
  
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.println(" *C");
  
  Serial.print("Hum: ");
  Serial.print(hum);
  Serial.println(" %");
  
  Serial.println();
  delay(1000);
}

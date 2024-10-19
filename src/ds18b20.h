#include <GyverDS18.h>
GyverDS18Single ds(2); //D4

void GetTemp() {
  if (ds.ready()) {         // измерения готовы по таймеру
    if (ds.readTemp()) {  // если чтение успешно
      tem = ds.getTemp();
      if (DBG) {
        Serial.print("[DBG] Temperature: ");
        Serial.print(tem);
        Serial.println("°C");
      }
    } else {
      Serial.println("DS18B20 error");
    }
    ds.requestTemp();
  }
}

void ds_handle(int ds_interval) {
  static unsigned long PrevDStime = 0;

  if (PrevDStime + ds_interval * 1000 < millis()) return;
  PrevDStime = millis();
  GetTemp();
  Serial.print("[DBG] Текущая температура: ");
  Serial.print(tem);
  Serial.println("°C");
}
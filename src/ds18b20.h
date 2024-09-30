#include <GyverDS18.h>
GyverDS18Single ds(2); //D4

void GetTemp() {
  if (ds.ready()) {         // измерения готовы по таймеру
    if (ds.readTemp()) {  // если чтение успешно
      tem = ds.getTemp();
      if (DBG) {
        Serial.print("temp: ");
        Serial.print(tem);
        Serial.println("°C");
      }
    } else {
      Serial.println("DS18B20 error");
    }
    ds.requestTemp();
  }
}

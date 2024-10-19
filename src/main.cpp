#include <Arduino.h>

#include <GyverPortal.h>
GyverPortal portal;

// #define RELAY 12 // D6
#define VibroPin 4 // D2 Pins for vibro-motor
#define BACKLIGHT_DAY 70  // Backlight for day
#define BACKLIGHT_NIGHT 10 // Backlight for night

#include "vars.h"       // Список переменных
// #include "pins.h"
#include "ds18b20.h"    // в этом файле работа с датчиком ds18b20
#include "ntptime.h"    // время и его обновление по NTP
#include "sunset.h"
#include "relay.h"      // в этом файле работа с реле
#ifdef disp_led7seg
  #include "led7seg.h"
#endif
#ifdef disp_max7219
  #include "max7219.h"
#endif
#ifdef disp_ssd1306
  #include "oled_ssd1306.h"
#endif
#include "romfunc.h"
#include "interface.h"
#include "actions.h"

void setup() {
  // delay(1000);
  pins_set();
  Serial.begin(115200);
  Serial.println("\nLED start");
  configModeCallback();
  if (!eeprom_read()) set_vars_start();
  Serial.println("WiFi start");
  ShowConnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  Serial.println("AP connect");
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  if (WiFi.isConnected()) {
    Serial.print(F("\nАдрес управления: "));
    Serial.println(WiFi.localIP());
  }

  // подключаем конструктор и запускаем
  portal.attachBuild(build);
  portal.attach(action);
  portal.start();
  NTP.setPeriod(ntp_req * 60);
  NTP.setHost(ntp_srv);
  NTP.begin(GMT_OFF);

  ds.requestTemp();
  if (WiFi.isConnected()) {
    //animPhrase("CONNECT ", 7);
    IP_Show();
  }
  GetTemp();
  if (tem) isDS18 = true;
}

void loop() {
  portal.tick();
  NTP.tick();
#ifdef disp_max7219
  disp.tick();
#endif
  if (NTP.newSecond()) { // новая секунда
    timer_handle(time_int);
    feed_handle(feed_int);
    night_handle(night_int);
    date_handle(YearTime * 1000);
    if (!YearShow) time_handle();
    relay();
  }
  if (isDS18) ds_handle(ds_int); // цикл замера температуры ds18b20
  else if (DBG) Serial.println("[DBG] Датчик температуры не найден");
  // GetTemp();
}

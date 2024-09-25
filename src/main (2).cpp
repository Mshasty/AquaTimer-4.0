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
#include "ntptime.h"
#include "sunset.h"
#include "relay.h"      // в этом файле работа с реле
#include "led7seg.h"
#include "romfunc.h"
#include "interface.h"
#include "actions.h"

void setup() {
  // delay(1000);
  pins_set();
  Serial.begin(115200);
  Serial.println("\nLED start");
	lc.shutdown(0, false); // Initialize LCD
	lc.setIntensity(0, .7); // Set the brightness to a low value
	lc.clearDisplay(0); // and clear the display
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
  if (WiFi.isConnected()) IP_Show();
}

void loop() {
  portal.tick();
  NTP.tick();
  if (NTP.newSecond()) { // новая секунда
  

    timer_handle(time_int);
    feed_handle(feed_int);
    night_handle(night_int);
    date_handle(YearTime * 1000);
    if (!YearShow) time_handle();
    relay();
  }
  // if (ds_int) 
  //ds_handle(ds_int); // цикл замера температуры ds18b20
  GetTemp();
}

#include <Arduino.h>


#define AP_SSID "Bikar6"
#define AP_PASS "htdjkzwbz1917"

#include <GyverPortal.h>
GyverPortal portal;

#define RELAY 12 // D6
#define VibroPin 4 // D2 Pins for vibro-motor
#define BACKLIGHT_DAY 70  // Backlight for day
#define BACKLIGHT_NIGHT 10 // Backlight for night

#include "vars.h"       // Список переменных
// #include "pins.h"
#include "ds18b20.h"    // в этом файле работа с датчиком ds18b20
#include "relay.h"      // в этом файле работа с реле
#include "timing.h"
#include "led7seg.h"
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
  Serial.println("WiFi start");
  ShowConnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(AP_SSID, AP_PASS);
  Serial.println("AP connect");
  set_vars_start();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if (WiFi.isConnected()) {
    Serial.print(F("\nАдрес управления: "));
    Serial.println(WiFi.localIP());
    IP_Show();
  }

  // подключаем конструктор и запускаем
  portal.attachBuild(build);
  portal.attach(action);
  portal.start();
}

void loop() {
  portal.tick();
  ds_handle(ds_int); // цикл замера температуры ds18b20
	if (time_setup) {
		time_set = update_handle(NTP_req * 60);
	} else {
		if (DBG) Serial.println("Первичный запрос NTP");
    time_set = update_handle(5);
    if (time_set) time_setup = true;
	}
	date_handle(YearTime * 1000);
	if (!YearShow) time_handle();
	relay();
}

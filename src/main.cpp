#include <Arduino.h>


#define AP_SSID "Bikar6"
#define AP_PASS "htdjkzwbz1917"

#include <GyverPortal.h>
GyverPortal portal;

#define RELAY 12 // D6

#include "vars.h"       // Список переменных
#include "ds18b20.h"    // в этом файле работа с датчиком ds18b20
#include "relay.h"      // в этом файле работа с реле
#include "timing.h"
#include "led7seg.h"

boolean t1d1, t1d2, t1d3, t1d4, t1d5, t1d6=true, t1d7=true;
boolean feed1ch=true;
boolean feed2ch;
boolean valSwitch;
String valNTPs="ntp5.stratum2.ru";
int valTZ=2;
float valFloat=3.14;
//char valPass[10];
int impuls=300;
int valNTPreq=30;
int prg=27;
int gsr=1;
// GPdate valDate;
GPtime Timer1start;
GPtime Timer1stop;
int days1=0;
int rt1=0;
uint8_t week_t1=63;
GPtime Timer2start;
GPtime Timer2stop;
int days2=0;
int rt2=1;
uint8_t week_t2=63;
GPtime Timer3start;
GPtime Timer3stop;
int days3=1;
int rt3=2;
uint8_t week_t3=60;
GPtime Timer4start;
GPtime Timer4stop;
int days4=2;
int rt4=3;
uint8_t week_t4=3;
GPtime feed1start, feed2start;
GPcolor valCol;
int valSelect;
//float valSpin;
boolean eff_clock=true;
int time_view=12;
int data_view=5;
int eff_speed=25;

#include "interface.h"
#include "actions.h"


void setup() {
  // delay(1000);
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
  Timer1start.hour = 9;
  Timer1start.minute = 30;
  Timer1stop.hour = 20;
  Timer1stop.minute = 30;
  Timer2start.hour = 16;
  Timer2start.minute = 15;
  Timer2stop.hour = 18;
  Timer2stop.minute = 40;
  Timer3start.hour = 11;
  Timer3start.minute = 00;
  Timer3stop.hour = 22;
  Timer3stop.minute = 00;
  Timer4start.hour = 15;
  Timer4start.minute = 30;
  Timer4stop.hour = 18;
  Timer4stop.minute = 40;
  feed1start.hour = 11;
  feed1start.minute = 20;
  feed2start.hour = 18;
  feed2start.minute = 45;
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

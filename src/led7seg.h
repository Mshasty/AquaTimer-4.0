#include <LedControl.h>           // https://github.com/wayoda/LedControl/releases  MAX7219 7 segment driver

#define delaytime 1000

/*For ESP-01 Don't Change GPIO Values
// GPIO0= DIN pin, GPIO1 = CLK pin, GPIO 2 = LOAD pin
LedControl lc = LedControl(0, 1, 2, 1)
//Reset Button Connected On GPIO3 and Ground
int inPin = 3;
*/

//For ESP-07 Tested GPIO Values
// GPIO 13 = DIN pin, GPIO 12 = CLK pin, GPIO 14 = LOAD pin. 
//For ESP-12E Tested GPIO Values
// GPIO 13 (D7) = DIN pin, GPIO 14 (D5) = CLK pin, GPIO 15 (D8) = LOAD pin. 
LedControl lc = LedControl(13, 14, 15, 1);
// the GPIO number For The "Reset" Push Button Switch Pin For ESp-07 Tested
//int inPin = 5;

boolean YearShow = false;       // Идёт показ даты

void digitalClockDisplay() {
  //tmElements_t tm;
  //char *dayOfWeek;
  //breakTime(now(), tm);
  lc.clearDisplay(0);
  int wd = weekday() - 1;
  if (!wd) wd = 7;
  lc.setDigit(0, 7, wd, false);
  lc.setChar(0, 6, ' ', false);
  lc.setDigit(0, 5, hour() / 10, false);
  lc.setDigit(0, 4, hour() % 10, true);
  lc.setDigit(0, 3, minute() / 10, false);
  lc.setDigit(0, 2, minute() % 10, true);
  lc.setDigit(0, 1, int(second() / 10), false);
  lc.setDigit(0, 0, second() % 10, false);
}

void ShowEffect() {
  static boolean ccw = false;
  lc.clearDisplay(0);
  if (ccw) {
    uint8_t kk = 2;
    for (uint8_t i = 1; i < 7; i++) {
      for (uint8_t j = 0; j < 8; j++)
        lc.setRow(0, j, kk);
      kk <<= 1;
      delay(80);
    }
    ccw=false;
  } else {
    uint8_t kk = 64;
    for (uint8_t i = 1; i < 7; i++) {
      for (uint8_t j = 0; j < 8; j++)
        lc.setRow(0, j, kk);
      kk >>= 1;
      delay(80);
    }
    ccw=true;
  }
}

void ShowEffect2() {
  if (eff_clock) {
    lc.clearDisplay(0);
    for (uint8_t j = 7; j > 0; j--) {
      lc.setRow(0, j, 2);
      if (j<6) lc.setRow(0, j+2, 0);
      delay(eff_speed);
      if (j<7) lc.setRow(0, j+1, 32);
      lc.setRow(0, j, 34);
      delay(eff_speed);
    }
    lc.setRow(0, 2, 0);
    lc.setRow(0, 0, 2);
    delay(eff_speed);
    lc.setRow(0, 0, 3);
    delay(eff_speed);
    lc.setRow(0, 1, 0);
    lc.setRow(0, 0, 5);
    delay(eff_speed);
    lc.setRow(0, 0, 4);
    for (uint8_t j = 1; j < 8; j++) {
      if (j>1) lc.setRow(0, j-2, 0);
      lc.setRow(0, j, 16);
      delay(eff_speed);
      lc.setRow(0, j-1, 4);
      lc.setRow(0, j, 20);
      delay(eff_speed);
    }
    lc.setRow(0, 6, 0);
    delay(eff_speed);
  }
}

void digitalDateDisplay() {
  ShowEffect();
  lc.setDigit(0, 7, day() / 10, false);
  lc.setDigit(0, 6, day() % 10, true);
  lc.setDigit(0, 5, month() / 10, false);
  lc.setDigit(0, 4, month() % 10, true);
  uint16_t yy = year();
  lc.setDigit(0, 3, yy / 1000, false);
  yy = yy % 1000;
  lc.setDigit(0, 2, yy / 100, false);
  yy = yy % 100;
  lc.setDigit(0, 1, yy / 10, false);
  lc.setDigit(0, 0, yy % 10, false);
}

void digitalTempDisplay() {
  ShowEffect();
  lc.clearDisplay(0);
  lc.setRow(0, 7, 0x0f);  // t
  lc.setDigit(0, 6, 0, false);   // 0
  lc.setChar(0, 5, ' ', false);
  lc.setDigit(0, 4, tem / 10, false);  
  lc.setDigit(0, 3, int(tem) % 10, true);  
  //uint16_t ttt = tem * 100 + 5; 
  lc.setDigit(0, 2, int((tem * 100 + 5) / 10) % 10, false);  
  lc.setRow(0, 1, 0b01100011); // gradus
  lc.setRow(0, 0, 0b01001110); // Celsium
}

void date_handle(unsigned long DateTimeView) {
  static unsigned long PrevDate = 0;
  static boolean LastDate = false; //

  if (YearTime > 5) {
    if (PrevDate + DateTimeView > millis())
      return;
    if (YearShow) {
      if (PrevDate + DateTimeView + 1000 * YearView < millis()) {
        if (DBG) Serial.println(F("*** End show Date/temp ***"));
        PrevDate = millis();
        YearShow = false;
        ShowEffect2();
      }
    } else {
      YearShow = true;
      if (LastDate) {
        if (DBG) {
          Serial.println(F("*** Show temp ***"));
          Serial.printf("Текущая температура: %.2f°C\r\n", tem);
        }
        digitalTempDisplay();
        LastDate = false;
      } else {
        if (DBG) Serial.println(F("*** Show Date ***"));
        digitalDateDisplay();
        LastDate = true;
      }
    }
  }
}

//To Display <Setup> if not connected to AP
//void configModeCallback (WiFiManager *myWiFiManager) {
void configModeCallback() {  
 	//lc.shutdown(0, false);

  lc.setChar(0, 7, ' ', false);
  lc.clearDisplay(0);
  lc.setRow(0, 6, 0x5b);        // S
  lc.setChar(0, 5, 'E', false); // E
  //lc.setRow(0, 5, 0x4f);        // E
  lc.setRow(0, 4, 0x0f);        // t
  lc.setRow(0, 3, 0x03e);       // u
  lc.setChar(0, 2, 'P', true);  // P
  lc.setChar(0, 1, ' ', true);
  lc.setChar(0, 0, ' ', true);
  delay(500);
}

//Display <StArt>
void ShowStart() {
  lc.setRow(0, 6, 0x5b);
  lc.setRow(0, 5, 0x0f);
  lc.setRow(0, 4, 0x77);
  lc.setRow(0, 3, 0x05);
  lc.setRow(0, 2, 0x0f);
  delay(500);
}

//Display <Connect> Once Connected to AP
void ShowConnect() {
  lc.setRow(0, 7, 0x4e);
  lc.setRow(0, 6, 0x7e);
  lc.setRow(0, 5, 0x76);
  lc.setRow(0, 4, 0x76);
  lc.setRow(0, 3, 0x4f);
  lc.setRow(0, 2, 0x4e);
  lc.setRow(0, 1, 0x0f);
  lc.setChar(0, 0, ' ', false);
  delay(300);  
}

//Display <Feeding> 
void ShowFeeding() {
  lc.setRow(0, 7, 0x47);
  lc.setRow(0, 6, 0x4f);
  lc.setRow(0, 5, 0x4f);
  lc.setRow(0, 4, 0x3d);
  lc.setRow(0, 3, 0x30);
  lc.setRow(0, 2, 0x15);
  lc.setRow(0, 1, 0x5e);
  lc.setChar(0, 0, ' ', false);
  delay(750);  
  lc.clearDisplay(0);
  lc.setRow(0, 2, 0x5B);
  if (FeedDelay > 10000) lc.setDigit(0, 6, int(FeedDelay/10000), false);
  lc.setDigit(0, 5, int(FeedDelay/1000) % 10, true);
  lc.setDigit(0, 4, int(FeedDelay/100) % 10, false);
  digitalWrite(VibroPin, !VibroUp);
  delay(FeedDelay);
  digitalWrite(VibroPin, VibroUp);
  if (FeedDelay < 750) delay(750 - FeedDelay);
}

void IP_Show() {
  uint8_t ipb[4];
  uint8_t ipab[3][4];
  uint32_t myip = WiFi.localIP();
  // uint32_t myip = my_ip.addr;

  //ipb[0] = ((uint8_t*)(my_ip.addr)[0]);
  ipb[0] = myip / 0x1000000;
  myip = myip % 0x1000000;
  ipb[1] = myip / 0x10000;
  myip = myip % 0x10000;
  ipb[2] = myip / 0x100;
  ipb[3] = myip % 0x100;
  for (uint8_t i=0; i<4; i++) {
    uint8_t ip_byte = ipb[i];
    ipab[0][i] = ip_byte / 100;
    ip_byte = ip_byte % 100;
    ipab[1][i] = ip_byte / 10;
    ipab[2][i] = ip_byte % 10;
  }
  lc.setRow(0,7,B00110000);  // I
  lc.setRow(0,6,B01100111);  // P
  for (uint8_t i=0; i<2; i++) {
    for (uint8_t j=0; j<2; j++) {
      lc.setChar(0,5,ipab[0][3 - 2 * j],false);  
      lc.setChar(0,4,ipab[1][3 - 2 * j],false);  
      lc.setChar(0,3,ipab[2][3 - 2 * j],true);  
      lc.setChar(0,2,ipab[0][2 - 2 * j],false);  
      lc.setChar(0,1,ipab[1][2 - 2 * j],false);  
      lc.setChar(0,0,ipab[2][2 - 2 * j],(j==0));  
      delay(delaytime);
    }
  }
  delay(delaytime);
  lc.clearDisplay(0);
}

void time_handle() {
  static time_t prevDisplay = 0;
  timeStatus_t ts = timeStatus();   
  //Serial.printf("Статус времени: %d\r\n", ts);
  switch (ts) {
    case timeNeedsSync:
    case timeSet:
      //update the display only if time has changed
      if (now() != prevDisplay) {
        prevDisplay = now();
        digitalClockDisplay();
        //tmElements_t tm;
        //breakTime(now(), tm);
        //If Time Needs Sync Display a "-" on second Digit
        if (ts == timeNeedsSync) {
          lc.setChar(0, 1, '-', false);
        }
      }
      break;
    case timeNotSet:
      //Display <No Sync> If Time Not Displayed
      lc.clearDisplay(0);
      lc.setRow(0, 7, 0x15);
      lc.setRow(0, 6, 0x1d);
      lc.setRow(0, 4, 0x5b);
      lc.setRow(0, 3, 0x3b);
      lc.setRow(0, 2, 0x15);
      lc.setRow(0, 1, 0x0d);
      now();
      delay(3000);
      //ESP.restart();
  } 
}

void night_handle(unsigned int del_int) {
  static unsigned long del_night;
  unsigned int time_now;
  unsigned int time_on;
  unsigned int time_off;
  float old_intens;
  float new_intens;

  if (!my_light.Night_sw) {
    NightMode = false;
    return;
  }
  if (del_night + del_int < millis()) { // Если интервал уже истек
    time_now = hour() * 60 + minute();
    time_on = my_light.Night_on.hour * 60 + my_light.Night_on.minute;
    time_off = my_light.Night_off.hour * 60 + my_light.Night_off.minute;
    old_intens = led_intens;
    if ((time_now >= time_on) || (time_now < time_off)) {
      new_intens = led_dark / 100;
      NightMode = true;
    } else {
      new_intens = led_light / 100;
      NightMode = false;
    }
    led_intens = new_intens;
    if (!(new_intens == old_intens)) {
      lc.setIntensity(0, new_intens);
      if (DBG) {}
      Serial.println("Смена режима день/ночь");
    }
    del_night = millis();
  }
}
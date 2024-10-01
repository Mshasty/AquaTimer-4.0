#include <GyverSegment.h>
#include <mString.h>

#define DIO_PIN 13
#define CLK_PIN 14
#define LAT_PIN 15

Disp7219<1> disp(DIO_PIN, CLK_PIN, LAT_PIN);

//SegRunner run(&disp);

const char pstr[] PROGMEM = "AquaTimer v.4.2";
//const char week[] = {32, 33, 34, 35, 36, 37, 38, 39};

void showPhrase(String Phrase) {
  disp.brightness(led_intens);
  disp.setCursor(0);
  disp.print(Phrase);
  disp.update();
  if (DBG) Serial.println(Phrase);
}

void animPhrase(String Phrase, int animNum = 5) {
  SegAnimation<8> anim(&disp, 0);

  switch (animNum) {
    case 1:
      anim.setEffect(SegEffect::Blink);
      break;
    case 2:
      anim.setEffect(SegEffect::RollUp);
      break;
    case 3:
      anim.setEffect(SegEffect::RollDown);
      break;
    case 4:
      anim.setEffect(SegEffect::TwistFill);
      break;
    case 5:
      anim.setEffect(SegEffect::TwistClear);
      break;
    case 6:
      anim.setEffect(SegEffect::SlideUp);
      break;
    case 7:
      anim.setEffect(SegEffect::SlideDown);
      break;
    case 8:
      anim.setEffect(SegEffect::None);
      break;
    default:
      anim.setEffect(SegEffect::None);
  }

  disp.printRight(false);
  anim.start();
  anim.setCursor(0);
  anim.print(Phrase);
  anim.waitEnd();
  disp.delay(100);
}

String stringClockDisplay() {
  mString<50> strClock;
  Datime dt(NTP);
  //strClock = dt.weekDay + " " + lz(dt.hour) + "." + lz(dt.minute) + "." + lz(dt.second);
  strClock = String(dt.weekDay) + " " + lz(dt.hour) + "." + lz(dt.minute) + "." + lz(dt.second);
  return(strClock.buf);
}

void digitalClockDisplay() {
  disp.brightness(led_intens);
  disp.setCursor(0);
  disp.print(stringClockDisplay());
  disp.update();
  if (DBG) Serial.println(stringClockDisplay());
}

void digitalDateDisplay() {
  disp.brightness(led_intens);
  showPhrase(NTP.dateToString());
}

void animateDateDisplay() {
  int animNum = random(5)+2;
  //animPhrase(NTP.dateToString(), animNum);
  animPhrase(stringClockDisplay(), animNum);
  if (DBG) {
    Serial.print("Animated date with ");
    Serial.println(animNum);
  }
}

void digitalTempDisplay() {
  /*mString<50> strTemp;
  strTemp = "t=";
  strTemp += (int(tem)) + ".";
  strTemp += (int((tem * 100 + 5) / 10) % 10);
  strTemp += (int((tem * 1000 + 5) / 10) % 10);
  strTemp += + "*C"; */
  disp.setCursor(0);
  //disp.print(strTemp.buf);
  disp.print("t=");
  disp.print(tem);
  disp.print("*C");
  disp.update();
  //Serial.println(strTemp.buf);
}

void IP_Show() {
  uint8_t ipb[4];
  uint32_t myip = WiFi.localIP();

  /* mString<50> strDisp;
  strDisp = "IP=";
  strDisp += "192.168.001.084";
  SegRunner run_local(&disp);
  run_local.setText(strDisp.buf);
  run_local.start();
  run_local.waitEnd();
  delay(300); */
  
  showPhrase("IPADRESS");
  
  ipb[0] = myip / 0x1000000;
  myip = myip % 0x1000000;
  ipb[1] = myip / 0x10000;
  myip = myip % 0x10000;
  ipb[2] = myip / 0x100;
  ipb[3] = myip % 0x100;

  disp.printRight(true);
  for (uint8_t i=0; i<2; i++) {
    disp.setCursorEnd();
    disp.fillChar('0');
    disp.print(ipb[2]);
    disp.setCursor(4);
    disp.print(ipb[3]);
    disp.setCursor(1);
    disp.print("IP");
    disp.point(4);
    disp.point(7);
    disp.update();
    disp.delay(1500);
    disp.setCursorEnd();
    disp.fillChar('0');
    disp.print(ipb[0]);
    disp.setCursor(4);
    disp.print(ipb[1]);
    disp.setCursor(1);
    disp.print("IP");
    disp.point(4);
    disp.update();
    disp.delay(1500);
  }
  disp.printRight(false);
  disp.clear();
}

void ShowDownload() {
  showPhrase("  LOAD  ");
  delay(500);
}

void ShowUpload() {
  showPhrase(" UPLOAD ");
  delay(500);
}

void ShowFeeding() {

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
        animateDateDisplay();
        //ShowEffect2();
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
        // if (ds_int) 
        LastDate = true;
        // else LastDate = false; // температуру не показываем
      }
    }
  }
}

void time_handle() {
  // static time_t prevDisplay = 0;
     
  if (NTP.online()) {
    //update the display only if time has changed
    digitalClockDisplay();
    if (DBG) Serial.println(NTP.timeToString());
  } else {
    //Display <No Sync> If Time Not Displayed
    showPhrase(" NOSYNC ");
    delay(3000);
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
    time_now = NTP.hour() * 60 + NTP.minute();
    time_on = my_light.Night_on.hour * 60 + my_light.Night_on.minute;
    time_off = my_light.Night_off.hour * 60 + my_light.Night_off.minute;
    old_intens = led_intens;
    if ((time_now >= time_on) || (time_now < time_off)) {
      new_intens = led_dark;
      NightMode = true;
    } else {
      new_intens = led_light;
      NightMode = false;
    }
    led_intens = new_intens;
    if (!(new_intens == old_intens)) {
#ifdef disp_led7seg
      lc.setIntensity(0, led_intens*10);
#else
      disp.brightness(led_intens);
#endif
      if (DBG) Serial.println("Смена режима день/ночь");
    }
    del_night = millis();
  }
}


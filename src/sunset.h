#include <Ticker.h>

Ticker blinker;

void noChangeIsr() { }

void daybreakIsr() {
  if (zakat_cur >= zakat_max) return;
  zakat_cur += zakat_step;
  if (zakat_cur > zakat_max) zakat_cur = zakat_max;
  if (zakat_inv) analogWrite(relays[zakat_rel], 1023 - zakat_cur);
  else analogWrite(relays[zakat_rel], zakat_cur);
  if (zakat_cur >= zakat_max) {
    Serial.println(strTimeNow() + " Рассвет окончен");
    blinker.attach(60, noChangeIsr);
  }
}

void sundownIsr() {
  if (zakat_cur <= zakat_min) return;
  zakat_cur -= zakat_step;
  if (zakat_cur < zakat_min) zakat_cur = zakat_min;
  if (zakat_inv) analogWrite(relays[zakat_rel], 1023 - zakat_cur);
  else analogWrite(relays[zakat_rel], zakat_cur);
  if (zakat_cur <= zakat_min) {
    digitalWrite(relays[zakat_rel], zakat_inv);
    Serial.println(strTimeNow() + " Закат окончен");
    blinker.attach(60, noChangeIsr);
  }
}
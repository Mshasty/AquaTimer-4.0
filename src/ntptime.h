#include <GyverNTP.h>

boolean time_setup = false;

void myDelay(uint16_t deltm) {
  unsigned long TimeStop = deltm + millis();
  while (millis() < TimeStop);      
}

String strTimeNow() {
    String strCurrentTime;

    strCurrentTime = NTP.timeToString();
    return strCurrentTime;
}
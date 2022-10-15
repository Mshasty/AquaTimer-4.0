#include <Time.h>                 // https://github.com/PaulStoffregen/Time
//#include <Timezone.h>             // https://github.com/JChristensen/Timezone
#include <NtpClientLib.h>

boolean wifiFirstConnected = false;
ipv4_addr my_ip;
String strIPaddr;
boolean time_setup = false;

//Edit These Lines According To Your Timezone and Daylight Saving Time
//TimeZone Settings Details https://github.com/JChristensen/Timezone
//TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120};     //Central European Time (Frankfurt, Paris)
//TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};       //Central European Time (Frankfurt, Paris)
//Timezone CE(CEST, CET);
//TimeChangeRule msk = {"MSK", Last, Sun, Mar, 1, 180};
//Timezone CE(msk);
//Pointer To The Time Change Rule, Use to Get The TZ Abbrev
//TimeChangeRule *tcr;
//time_t utc ; 

// Start NTP only after IP network is connected
void onSTAGotIP (WiFiEventStationModeGotIP ipInfo) {
  strIPaddr = ipInfo.ip.toString().c_str();
  Serial.printf("DHCP issued IP: %s\r\n", ipInfo.ip.toString().c_str());
  Serial.printf("WiFi connected: %s\r\n", WiFi.status() == WL_CONNECTED ? "yes" : "no");
  //digitalWrite (ONBOARDLED, LOW); // Turn on LED
  wifiFirstConnected = true;
  my_ip = ipInfo.ip;
}

void processSyncEvent(NTPSyncEvent_t ntpEvent) {
    if (ntpEvent)
    {
        Serial.println("Ошибка NTP: ");
        if (ntpEvent == noResponse)
            Serial.println("Сервер не ответил");
        else if (ntpEvent == invalidAddress)
            Serial.println("Неверный адрес NTP сервера");
    }
    else
    {
        Serial.println("Получено время NTP: ");
        Serial.println(NTP.getTimeDateString(NTP.getLastNTPSync()));
    }
}

boolean ntp(int interval, String NTPhost, int8_t timeZone) {
    static boolean ntp_setup;
    if (!WiFi.isConnected())
        return false;
    if (!ntp_setup) {
        NTP.begin(NTPhost, timeZone, true, 0);
        NTP.setInterval(interval);
        ntp_setup = true;
        if (DBG) {
            Serial.print("собственно, запрос на ");
            Serial.println(NTPhost);
        }
    }
    if (DBG) Serial.println(NTP.getTimeDateString() + " Включено: " + NTP.getUptimeString() + " Обновлено: " + NTP.getTimeDateString(NTP.getFirstSync()).c_str());
    return ntp_setup;
}

boolean update_handle(int interval) {
    static unsigned long timer;
    static unsigned long interval_;
    boolean ts = false;


    interval_ = 1000 * interval;
    if (timer + interval_ < millis()) { // Если интервал еще не истек
        if (DBG) {
            Serial.print("Интервал: ");
            Serial.println(interval_);
            Serial.println("Запрос NTP");
        }
        timer = millis();
        // ntp_setup = false;
        ts = ntp(ntp_req, ntp_srv, GMT_OFF); // время NTP
    }
    return ts;
}

void myDelay(uint16_t deltm) {
  unsigned long TimeStop = deltm + millis();
  while (millis() < TimeStop);      
}

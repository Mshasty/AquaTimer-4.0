#include <EEPROM.h>
// #include <string.h>

struct EEPROM_Data {
  byte success;
  unsigned int ds_int; // период опроса ds18b20
  float p_tem; // порог срабатывания термостата
  float h_tem; // гистерезис термостата
  int valSelect; // режим первого реле
  uint8_t YearTime; // Период показа даты
  uint8_t YearView;  // Время показа даты
  int led_light; // яркость LED-индикатора
  int gmt_off; // Тайм-зона GMT
  int ntp_req; // Период запроса NTP-сервера
  boolean eff_clock; // показ эффекта перед часами
  int eff_speed; // замедление эффекта дисплея
  int FeedDelay; // длительность импульса кормления
  // boolean RelayUp; // инверсия реле
  // boolean BeepOnOff; // Бипер on/off
  // boolean VibroUp; // инверсия управления мотором
  char ntp_srv[30];
  Feed_set feeds[2];
  Timer_set my_timer[timers_num];
};

void eeprom_write() {
  EEPROM_Data data;

  data.success = 0xA5;
  data.ds_int = ds_int; 
  data.p_tem = p_tem; 
  data.h_tem = h_tem; 
  data.valSelect = valSelect;
  data.YearTime = YearTime;
  data.YearView = YearView;
  data.led_light = led_light;
  data.gmt_off = GMT_OFF;
  data.ntp_req = ntp_req;
  data.eff_clock = eff_clock;
  data.eff_speed = eff_speed;
  data.FeedDelay = FeedDelay;
  for (uint8_t i = 0; i < 30; i++) { // NTP-сервер
  // for (uint8_t i = 0; i < (strlen(ntp_srv) + 1); i++) {  
    if (i < sizeof(ntp_srv)) data.ntp_srv[i] = ntp_srv[i];
    else data.ntp_srv[i] = 0;
  }
  for (uint8_t i = 0; i < 2; i++) data.feeds[i] = Feeds[i];
  for (uint8_t i = 0; i < timers_num; i++) data.my_timer[i] = my_timer[i];

  if (DBG) {
    Serial.print("Размер используемого EEPROM ");
    Serial.print(sizeof(data));
    Serial.println(" байт");
  }
  
  EEPROM.begin(sizeof(data));
  delay(300);
  EEPROM.put(0, data);
  EEPROM.commit();
}

boolean eeprom_read() {
  EEPROM_Data data;
  uint8_t j;
  // char my_ntp_srv[30];

  if (DBG) {
    Serial.print("Размер используемого EEPROM ");
    Serial.print(sizeof(data));
    Serial.println(" байт");
  }

  EEPROM.begin(sizeof(data));
  delay(250);
  EEPROM.get(0, data);

  if (DBG) {
    Serial.print("Контроль данных (0xA5): ");
    Serial.println(data.success);
  }
  if (data.success != 0xA5) return false;
  ds_int = data.ds_int;
  p_tem = data.p_tem;
  h_tem = data.h_tem;
  valSelect = data.valSelect;
  if (valSelect==1) {
    mode = "TEM";
  } else if (valSelect==2) {
    mode = "MAN";
  } else {
    mode = "TMR";
  }
  YearTime = data.YearTime;
  YearView = data.YearView;
  led_light = data.led_light;
  led_intens = led_light/100;
  lc.setIntensity(0, led_intens);
  GMT_OFF = data.gmt_off;
  ntp_req = data.ntp_req;
  eff_clock = data.eff_clock;
  eff_speed = data.eff_speed;
  FeedDelay = data.FeedDelay;

  for (j = 0; j < 30; j++) {
    ntp_srv[j] = data.ntp_srv[j];
    //if(data.ntp_srv[j]==0) break;
  }
  if (DBG) {
    Serial.print("Полученный адрес NTP-сервера  ");
    Serial.println(ntp_srv);
  }
  for (uint8_t i = 0; i < 2; i++) Feeds[i] = data.feeds[i];
  for (uint8_t i = 0; i < timers_num; i++) {
    my_timer[i] = data.my_timer[i];
    my_timer[i].Timer_week = weekday_set[my_timer[i].Timer_days];
  }

  return true;
}

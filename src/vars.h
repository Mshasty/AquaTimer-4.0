// глобальные переменные для работы с ними в программе
uint8_t relays[4] = {12, 0, 5, 16};
uint8_t relay_num = 4;
uint8_t timers_num = 5;

unsigned int ds_int=5; // интервал замера ds18b20 в секундах 
unsigned int mqtt_int=30; // интервал отправки данных по MQTT в секундах 
float tem; // тут храним температуру
float p_tem=26; // температура включения реле
float h_tem=1; // гистерезис
String mode = "TMR"; // режим работы по температуре/ручной ("TEM"/"MAN/TMR")
String ntp_srv="ntp6.ntp-servers.net";
int tz=2; // Тайм-зона
// boolean ch1_on;
// boolean ch2_on;
boolean relay_on;
String tm[2];
uint8_t YearTime=12; // Период показа даты
uint8_t YearView=5;  // Время показа даты
int led_light;
unsigned int NTP_req=30; // Период запроса NTP-сервера
boolean eff_time = true; // показ эффекта перед часами
unsigned int td = 25; // скорость эффекта перед часами
boolean mqtt_enable = false;
boolean ntp_setup=false;
boolean time_set=false;
String temp;
float led_intens; // яркость 7-сегм.индикатора
boolean tempOK = true;
boolean valSwitch;
String valNTPs="ntp5.stratum2.ru";
int valTZ=2;
//float valFloat=3.14;
int FeedDelay=300;
int valNTPreq=30;
int prg=27;
int gsr=1;
GPtime Timer_start[10];
GPtime Timer_stop[10];
int Timer_days[10] = {0, 1, 2, 3, 4, 0, 1, 2, 3, 4};
int Timer_relay[10] = {0, 1, 2, 3, 4, 0, 1, 2, 3, 4};
uint8_t Timers_week[10];
// weekday_set = √√√√√√√,√√√√√∙∙,∙∙∙∙∙√√,√∙√∙√∙√,∙√∙√∙√∙
uint8_t weekday_set[5] = {127, 124, 3, 85, 42};
// GPtime feed1start, feed2start;
boolean feed_sw[2] = {true, false};
GPtime feed_start[2];
GPcolor valCol;
int valSelect;
boolean eff_clock=true;
int time_view=12;
int data_view=5;
int eff_speed=25;

boolean DBG = false; // Переменная для дебаг-сообщений
boolean DBG_portal = true; // Переменная для дебага портала

void set_vars_start() {
  Timer_start[0].hour = 9;
  Timer_start[0].minute = 30;
  Timer_stop[0].hour = 20;
  Timer_stop[0].minute = 30;
  Timer_start[1].hour = 16;
  Timer_start[1].minute = 15;
  Timer_stop[1].hour = 18;
  Timer_stop[1].minute = 40;
  Timer_start[2].hour = 11;
  Timer_start[2].minute = 00;
  Timer_stop[2].hour = 22;
  Timer_stop[2].minute = 00;
  Timer_start[3].hour = 15;
  Timer_start[3].minute = 30;
  Timer_stop[3].hour = 18;
  Timer_stop[3].minute = 40;
  Timer_start[4].hour = 12;
  Timer_start[4].minute = 15;
  Timer_stop[4].hour = 16;
  Timer_stop[4].minute = 25;
  // feed1start.hour = 11;
  // feed1start.minute = 20;
  // feed2start.hour = 18;
  // feed2start.minute = 45;
}

void pins_set() {
  for (uint8_t i = 0; i < relay_num; i++) {
    pinMode(relays[i], OUTPUT);
    digitalWrite(relays[i], LOW);
  }
  pinMode(VibroPin, OUTPUT);
  digitalWrite(VibroPin, LOW);
}


String day_week(int dw) {
  static String week_str[8] = {"cб", "вс", "пн", "вт", "ср", "чт", "пт", "сб"};
  return week_str[dw];
}

String lz(int val) {
    if (val<10) return "0" + String(val);
    else return String(val);
}

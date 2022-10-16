#define DBG false // Переменная для дебаг-сообщений
#define DBG_portal true // Переменная для дебага портала

// глобальные переменные для работы с ними в программе
static uint8_t relays[relay_num] = {12, 0, 5, 16};
boolean RelayUp = false; // инверсия реле
boolean VibroUp = false; // инверсия вибромотора
boolean ChOnOff[relay_num];
boolean BeepOnOff = false;
unsigned int ds_int = 5; // интервал замера ds18b20 в секундах
unsigned int time_int = 1500; // интервал опроса таймеров в милисекундах
unsigned int feed_int = 3000; // интервал опроса фидеров в милисекундах
// unsigned int mqtt_int=30; // интервал отправки данных по MQTT в секундах 
float tem; // тут храним температуру
int p_tem=26; // температура включения реле
int h_tem=1; // гистерезис
String mode = "TMR"; // режим работы по температуре/ручной ("TEM"/"MAN/TMR")
char ntp_srv[30] = "ntp6.ntp-servers.net";
int GMT_OFF=2; // Тайм-зона
int YearTime=12; // Период показа даты
int YearView=5;  // Время показа даты
int led_light;
int ntp_req=25; // Период запроса NTP-сервера
// boolean mqtt_enable = false;
boolean ntp_setup=false;
boolean time_set=false;
String temp;
float led_intens; // яркость 7-сегм.индикатора
boolean tempOK = true;
int FeedDelay=300;
int valNTPreq=30;
int prg=27;
int gsr=1;

struct Timer_set {
  GPtime Timer_start;
  GPtime Timer_stop;
  int Timer_days;
  int Timer_relay;
  uint8_t Timer_week;
};
Timer_set my_timer[10];
uint8_t weekday_set[6] = {0x7F, 0x1F, 0x60, 0x55, 0x2A, 0};
struct Feed_set {
  boolean feed_sw;
  GPtime feed_start;
};
Feed_set Feeds[2];
boolean feedOK;
boolean valSwitch;
int valSelect;
boolean eff_clock=true; // показ эффекта перед часами
int eff_speed = 25; // скорость эффекта перед часами

void set_vars_start() {
  int Timer_start_hour[10] = {9, 16, 11, 15, 12, 17, 13, 10, 20, 14};
  int Timer_start_minute[10] = {30, 15, 0, 30, 15, 0, 0, 0, 0, 0};
  int Timer_stop_hour[10] = {20, 18, 22, 16, 14, 20, 18, 13, 20, 16};
  int Timer_stop_minute[10] = {30, 40, 0, 25, 15, 0, 0, 0, 45, 0};
  int Timer_days[10] = {0, 1, 2, 3, 4, 0, 1, 2, 3, 4};
  uint8_t Timer_week[10] = {0x7F, 0x1F, 0x60, 0x55, 0x2A, 0x7F, 0x1F, 0x60, 0x55, 0x2A};
  int Timer_relay[10] = {0, 1, 2, 3, 0, 1, 2, 3, 0, 1};

  boolean feed_sw[2] = {true, false};
  int feed_hour[2] = {11, 18};
  int feed_minute[2] = {20, 45};

  for (uint8_t i = 0; i < timers_num; i++) {
    my_timer[i].Timer_start.hour = Timer_start_hour[i];
    my_timer[i].Timer_start.minute = Timer_start_minute[i];
    my_timer[i].Timer_stop.hour = Timer_stop_hour[i];
    my_timer[i].Timer_stop.minute = Timer_stop_minute[i];
    my_timer[i].Timer_days = Timer_days[i];
    my_timer[i].Timer_relay = Timer_relay[i];
    my_timer[i].Timer_week = Timer_week[i];
  }
  for (uint8_t i = 0; i < 2; i++) {
    Feeds[i].feed_start.hour = feed_hour[i];
    Feeds[i].feed_start.minute = feed_minute[i];
    Feeds[i].feed_sw = feed_sw[i];
  }
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

boolean XOR(boolean x, boolean y)
{
    return (x + y) % 2;
}
